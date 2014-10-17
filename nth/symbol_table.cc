#include <stdexcept>

#include "symbol_table.h"
#include "ast.h"

using namespace nth;

Symbol *SymbolTable::findSymbol(Identifier *ident) {
  for (auto it=scopes.rbegin(); it != scopes.rend(); ++it) {
    auto scope = *it;
    for (auto symbolit = scope.rbegin(); symbolit != scope.rend(); ++symbolit) {
      if(**symbolit == ident) return *symbolit;
    }
  }
  return nullptr;
}

bool SymbolTable::checkSymbol(Identifier *ident) {
  if (scopes.empty()) {
    pushScope();
  }

  Scope current = scopes.back();

  for (auto it=current.rbegin(); it != current.rbegin(); ++it) {
    if(**it == ident) return true;
  }
  return false;
}

void SymbolTable::pushScope() {
  scopes.push_back(Scope());
}

void SymbolTable::popScope() {
  if (scopes.empty()) throw std::runtime_error("symbol table is already empty");
  scopes.pop_back();
}

void SymbolTable::addSymbol(Identifier *ident) {
  if (scopes.empty()) {
    pushScope();
  }

  if (checkSymbol(ident)) {
    // TODO: handle this error correctly
    throw std::runtime_error("symbol is already defined in this scope");
  }
  //incrementscopesize();


  scopes.back().push_back(new Symbol(ident));
  ident->setSymbol(scopes.back().back());
}

Symbol::Symbol(Identifier *ident) : name(ident->getValue()) {}

bool Symbol::operator==(Identifier *ident) {
  return name == ident->getValue();
}

bool Symbol::operator==(std::string str) {
  return name == str;
}

std::deque<nth::Symbol*> SymbolTable::getSymbols() {
  std::deque<nth::Symbol*> symbols;
  for (auto it=scopes.begin(); it != scopes.end(); ++it) {
    auto scope = *it;
    for (auto symbolit = scope.begin(); symbolit != scope.end(); ++symbolit) {
      symbols.push_back(*symbolit);
    }
  }
  return symbols;
}

std::deque<SymbolTable::Scope> &SymbolTable::getScopes() {
  return scopes;
}

