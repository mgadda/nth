#include "scope_checker.h"
#include "type.h"

using namespace nth;

bool ScopeChecker::isValid() {
  return unknownIdentifiers.empty();
}

IdentifierList &ScopeChecker::getUnknownIdentifiers() {
  return unknownIdentifiers;
}

// * - introduces new symbol through an identifier
// † - references identifier (type or variable)
//
//
//void ScopeChecker::visit(nth::Identifier *ident) {}
//
//void ScopeChecker::visit(nth::FieldAccess *field_access) {
//  field_access->getLeftValue()->accept(*this);
//  // Cannot check this identifier without knowing 
//  // the resolved type of left hand expression
//  // but still need to traverse tree
//  field_access->getRightValue()->accept(*this);
//}
//
//void ScopeChecker::visit(nth::TupleFieldAccess *tuple_field_access) {
//  tuple_field_access->getLeftValue()->accept(*this);
//  tuple_field_access->getRightValue()->accept(*this);
//}
//
//void ScopeChecker::visit(nth::FunctionDef *functionDef) {
//  // Always push the symbol after defining the function name, otherwise
//  // this function can only be referenced in its own body
//  symbols.addSymbol(functionDef->getName());
//  symbols.pushFrame();
//
//  for (auto typeParam : functionDef->getTypeParameters()) {
//    typeParam->accept(*this);
//  }
//  for (auto arg : functionDef->getArguments()) {
//    arg->accept(*this);
//  }
//  functionDef->getReturnType()->accept(*this);
//  functionDef->getBlock()->accept(*this);
//
//  symbols.popFrame();
//} // *†
//
//void ScopeChecker::visit(nth::LambdaDef *lambdaDef) {} // *
//void ScopeChecker::visit(nth::FunctionCall *functionCall) {
//  functionCall->getCallable()->accept(*this);
//  for (auto argExpr : functionCall->getArguments()) {
//    argExpr->accept(*this);
//  }
//}
//
//void ScopeChecker::visit(nth::VariableDef *variableDef) {
//  symbols.addSymbol(variableDef->getName());
//} // *
//
//void ScopeChecker::visit(nth::Argument *argument) {
//  symbols.addSymbol(argument->getName());
//  argument->getType()->accept(*this);
//}
//
//void ScopeChecker::visit(nth::IfElse *ifElse) {}
//
//void ScopeChecker::visit(nth::SimpleTypeRef *type) {
//  // TODO: associate symbols with identifiers in some useful way
//  if (!symbols.findSymbol(type->getName())) {
//    unknownIdentifiers.push_back(type->getName());
//  }
//}
//
//void ScopeChecker::visit(nth::SimpleTypeDef *type) {
//  symbols.addSymbol(type->getName());
//}
//
//void ScopeChecker::visit(nth::TemplatedTypeRef *type) {
//
//  if (!symbols.findSymbol(type->getName())) {
//    unknownIdentifiers.push_back(type->getName());
//  }
//
//  for (auto type : type->getSubtypes()) {
//    type->accept(*this);
//  }
//}
//
//void ScopeChecker::visit(nth::TemplatedTypeDef *type) {
//  symbols.addSymbol(type->getName());
//  for (auto type : type->getSubtypes()) {
//    type->accept(*this);
//  }
//}
//
//void ScopeChecker::visit(nth::TypeAliasDef *typeAliasDef) {} // *

// --------------------------------
// Function name scope checking pass

void ScopeChecker::visit(nth::Identifier *ident) {}
void ScopeChecker::visit(nth::FieldAccess *field_access) {}
void ScopeChecker::visit(nth::TupleFieldAccess *tuple_field_access) {}
void ScopeChecker::visit(nth::FunctionDef *functionDef) {
  symbols.addSymbol(functionDef->getName());
  symbols.pushScope();
  Visitor::visit(functionDef);
  symbols.popScope();
}
void ScopeChecker::visit(nth::LambdaDef *lambdaDef) {}
void ScopeChecker::visit(nth::FunctionCall *functionCall) {}
void ScopeChecker::visit(nth::VariableDef *variableDef) {}
void ScopeChecker::visit(nth::Argument *argument) {}
void ScopeChecker::visit(nth::IfElse *ifElse) {}
void ScopeChecker::visit(nth::SimpleTypeRef *type) {}
void ScopeChecker::visit(nth::SimpleTypeDef *type) {}
void ScopeChecker::visit(nth::TemplatedTypeRef *type) {}
void ScopeChecker::visit(nth::TemplatedTypeDef *type) {}
void ScopeChecker::visit(nth::TypeAliasDef *typeAliasDef) {}


