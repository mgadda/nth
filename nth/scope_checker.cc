#include "scope_checker.h"
#include "type.h"

using namespace nth;

ScopeChecker::ScopeChecker() {
  symbolTables.push(new SymbolTable);
}

ScopeChecker::ScopeChecker(SymbolTable &table) {
  symbolTables.push(&table);
}

bool ScopeChecker::run(ASTNode *node) {
  if (!node) {
    throw new std::runtime_error("unrecoverable error: parse tree was empty");
  }

  ScopeChecker::ScopePrimer primer(*this);
  ScopeChecker::DeclareBeforeUse declareBeforeUse(*this);
  node->accept(primer);
  node->accept(declareBeforeUse);
  return isValid(); // TODO: make this not a constant
}

bool ScopeChecker::isValid() {
  return unknownIdentifiers.empty();
}

IdentifierList &ScopeChecker::getUnknownIdentifiers() {
  return unknownIdentifiers;
}

void ScopeChecker::withNewScope(ASTNode *node, std::function<void(SymbolTable&)> scopedFun) {
  symbolTables.push(symbolTables.top()->beget());
  node->setSymbolTable(symbolTables.top());
  scopedFun(*symbolTables.top());
  symbolTables.pop();
}

void ScopeChecker::withScope(nth::ASTNode *node, std::function<void(SymbolTable&)>scopedFun) {

  symbolTables.push(node->getSymbolTable());
  scopedFun(*symbolTables.top());
  symbolTables.pop();
}

SymbolTable &ScopeChecker::getSymbolTable(nth::ASTNode *node) {
  return *symbolTables.top();
}

// --------------------------------
// Function name scope checking pass
// --------------------------------
// Goal: find all function, class names and type aliases
// and save them into current scope

ScopeChecker::ScopePrimer::ScopePrimer(ScopeChecker &checker)
: scopeChecker(checker) {}

void ScopeChecker::ScopePrimer::visit(Block *block) {
  scopeChecker.withNewScope(block, [&](SymbolTable &table) {
    Visitor::visit(block);
  });
}

void ScopeChecker::ScopePrimer::visit(nth::FunctionDef *functionDef) {
  // A function definition introduces both a new symbol into its outer scope
  // and a scope of its own.

  scopeChecker.symbolTables.top()->addSymbol(functionDef->getName());

  scopeChecker.withNewScope(functionDef, [&](SymbolTable &table) {
    Visitor::visit(functionDef);
  });
}

void ScopeChecker::ScopePrimer::visit(nth::LambdaDef *lambdaDef) {
  // Lambda definitions do not introduce new symbols in the scope in which
  // they're defined. However, they introduce a new scope which needs to
  // be attached to a symbol table.
  scopeChecker.withNewScope(lambdaDef, [&](SymbolTable &table) {
    Visitor::visit(lambdaDef);
  });
}

void ScopeChecker::ScopePrimer::visit(nth::TypeAliasDef *typeAliasDef) {
  // Like function definitions, type Aliases alter the current scope
  // However, they do not introduce a new scope of their own since
  // they do not contain a body of statements.

  SymbolTable *currentTable = scopeChecker.symbolTables.top();
  currentTable->addSymbol(typeAliasDef->getLType()->getName());

  typeAliasDef->setSymbolTable(currentTable);
  typeAliasDef->getRType()->accept(*this);
}

// ----------------------------------

ScopeChecker::DeclareBeforeUse::DeclareBeforeUse(ScopeChecker &checker)
: scopeChecker(checker) {}

void ScopeChecker::DeclareBeforeUse::visit(nth::Block *block) {
  scopeChecker.withScope(block, [&](SymbolTable &table) {
    Visitor::visit(block);
  });
}

void ScopeChecker::DeclareBeforeUse::visit(nth::Identifier *ident) {
  // if we get here, it's because this use of an identitifer
  // is not associated with any sort of definition
  if (!scopeChecker.getSymbolTable(ident).findSymbol(ident)) {
    scopeChecker.unknownIdentifiers.push_back(ident);
  }

}

// TODO: enable once type checking / inference has been implemented so
// that we can verify field name refers to valid method for the type of
// expression associated with the left hand side.

//void ScopeChecker::DeclareBeforeUse::visit(nth::FieldAccess *field_access) {
//  field_access->getLeftValue()->accept(*this);
//
//  nth::Identifier &fieldName = field_access->getField();
//  SymbolTable &table = scopeChecker.getSymbolTable(&fieldName);
//
//  if (!table.findSymbol(&fieldName)) {
//    scopeChecker.unknownIdentifiers.push_back(&fieldName);
//  }
//}

void ScopeChecker::DeclareBeforeUse::visit(nth::FieldAccess *field_access) {
  field_access->getLeftValue()->accept(*this);
  // do not traverse field because we can't know if the field represents
  // a valid method without knowing the type of expression
  // TODO: determine if this means scope checking must occur after type inference
}


void ScopeChecker::DeclareBeforeUse::visit(nth::FunctionDef *functionDef) {
  // KEY: no need to create new symbol tables for scope
  // already done by first pass!
  scopeChecker.withScope(functionDef, [&](SymbolTable &table) {
    for (auto typeParam : functionDef->getTypeParameters()) {
      typeParam->accept(*this);
    }
    for (auto arg : functionDef->getArguments()) {
      arg->accept(*this);
    }
    functionDef->getReturnType()->accept(*this);
    functionDef->getBlock()->accept(*this);
  });
}

void ScopeChecker::DeclareBeforeUse::visit(nth::LambdaDef *lambdaDef) {
  scopeChecker.withScope(lambdaDef, [&](SymbolTable &table) {
    for (auto arg : lambdaDef->getArguments()) {
      arg->accept(*this);
    }
    lambdaDef->getReturnType()->accept(*this);
    lambdaDef->getBody()->accept(*this);
  });
}

void ScopeChecker::DeclareBeforeUse::visit(nth::VariableDef *variableDef) {
  scopeChecker.getSymbolTable(variableDef).addSymbol(variableDef->getName());
  variableDef->getVarType()->accept(*this);
  variableDef->getValue()->accept(*this);
}

void ScopeChecker::DeclareBeforeUse::visit(nth::Argument *argument) {
  scopeChecker.getSymbolTable(argument).addSymbol(argument->getName());
  argument->getType()->accept(*this);
}

void ScopeChecker::DeclareBeforeUse::visit(nth::SimpleTypeRef *type) {
  if (!scopeChecker.getSymbolTable(type).findSymbol(type->getName())) {
    scopeChecker.unknownIdentifiers.push_back(type->getName());
  }
}

void ScopeChecker::DeclareBeforeUse::visit(nth::SimpleTypeDef *type) {
  scopeChecker.getSymbolTable(type).addSymbol(type->getName());
}

void ScopeChecker::DeclareBeforeUse::visit(nth::TemplatedTypeRef *type) {

  if (!scopeChecker.getSymbolTable(type).findSymbol(type->getName())) {
    scopeChecker.unknownIdentifiers.push_back(type->getName());
  }

  for (auto type : type->getSubtypes()) {
    type->accept(*this);
  }
}

void ScopeChecker::DeclareBeforeUse::visit(nth::TemplatedTypeDef *type) {
  scopeChecker.getSymbolTable(type).addSymbol(type->getName());
  for (auto type : type->getSubtypes()) {
    type->accept(*this);
  }
}



