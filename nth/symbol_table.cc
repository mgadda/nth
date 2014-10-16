#include <stdexcept>

#include "symbol_table.h"
#include "ast.h"

using namespace nth;

Symbol *SymbolTable::findSymbol(Identifier *ident) {
  for (auto it=symbols.rbegin(); it != symbols.rend(); ++it) {
    if(**it == ident) return *it;
  }
  return nullptr;
}

bool SymbolTable::checkSymbol(Identifier *ident) {
  for (auto it=symbols.rbegin(); it != symbols.rbegin() + framesizes.top(); ++it) {
    if(**it == ident) return true;
  }
  return false;
}

void SymbolTable::pushFrame() {
  framesizes.push(0);
}

void SymbolTable::popFrame() {
  if (framesizes.empty()) throw std::runtime_error("symbol table is already empty");
  
  int size = framesizes.top();
  framesizes.pop();
  for (int i = 0; i < size; ++i) symbols.pop_back();
}

void SymbolTable::addSymbol(Identifier *ident) {
  if (checkSymbol(ident)) {
    // TODO: handle this error correctly
    throw std::runtime_error("symbol is already defined in this scope");
  }
  incrementFramesize();
  symbols.push_back(new Symbol(ident));
  ident->setSymbol(symbols.back());
}

void SymbolTable::incrementFramesize() {
  if (framesizes.empty()) {
    framesizes.push(1);
  } else {
    int size = framesizes.top();
    framesizes.pop();
    framesizes.push(size+1);
  }
}

Symbol::Symbol(Identifier *ident) : name(ident->getValue()) {}

bool Symbol::operator==(Identifier *ident) {
  return name == ident->getValue();
}

bool Symbol::operator==(std::string str) {
  return name == str;
}
