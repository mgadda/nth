#include <stdexcept>

#include "symbol_table.h"
#include "ast.h"

using namespace nth;

SymbolTable::SymbolTable() {}
SymbolTable::SymbolTable(SymbolTable *parent) : parent(parent) {}

SymbolTable *SymbolTable::beget() {
  return new SymbolTable(this);
}

Symbol *SymbolTable::findLocalSymbol(Identifier *ident) {
  for (auto it=scope.rbegin(); it != scope.rend(); ++it) {
    if(**it == ident) return *it;
  }
  return nullptr;
}

Symbol *SymbolTable::findSymbol(Identifier *ident) {
  if (Symbol *symbol = findLocalSymbol(ident)) {
    return symbol;
  } else if (parent != nullptr) {
    return parent->findSymbol(ident);
  } else {
    return nullptr;
  }

//  for (auto it=scopes.rbegin(); it != scopes.rend(); ++it) {
//    auto scope = *it;
//    for (auto symbolit = scope.rbegin(); symbolit != scope.rend(); ++symbolit) {
//      if(**symbolit == ident) return *symbolit;
//    }
//  }
//  return nullptr;
}

bool SymbolTable::checkSymbol(Identifier *ident) {
  if (findLocalSymbol(ident) != nullptr) {
    return true;
  }
  return false;
}

void SymbolTable::addSymbol(Identifier *ident) {
  // if symbol has already been defined, raise error
  if (checkSymbol(ident)) {
    // TODO: handle this error correctly
    throw std::runtime_error(ident->getValue() + " is already defined in this scope");
  }

  scope.push_back(new Symbol(ident));
}


std::deque<nth::Symbol*> SymbolTable::getSymbols() {
  std::deque<nth::Symbol*> symbols;

  for (auto symbolit = scope.begin(); symbolit != scope.end(); ++symbolit) {
    symbols.push_back(*symbolit);
  }

  if (parent) {
    auto parentSymbols = parent->getSymbols();

    symbols.insert(symbols.begin(), parentSymbols.begin(), parentSymbols.end());
  }

  return symbols;
}

std::list<SymbolTable*> SymbolTable::getScopes() {
  std::list<SymbolTable*> scopes(1, this);
  if (parent) {
    auto parentScopes = parent->getScopes();
    scopes.insert(scopes.begin(), parentScopes.begin(), parentScopes.end());
  }

  return scopes;
}

// -------------------------
// Symbol method definitions
// -------------------------

Symbol::Symbol(Identifier *ident) : name(ident->getValue()) {}

bool Symbol::operator==(Identifier *ident) {
  return name == ident->getValue();
}

bool Symbol::operator==(std::string str) {
  return name == str;
}
