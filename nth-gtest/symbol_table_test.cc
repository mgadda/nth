#include <stdexcept>
#include <gtest/gtest.h>
#include "test_helper.h"
#include "symbol_table.h"
#include "ast.h"
#include <stack>

namespace nth {
  class Symbol;
}

class SymbolTableTest : public ::testing::Test {

protected:
  nth::SymbolTable root;
  virtual void SetUp() {}
};

TEST_F(SymbolTableTest, FindsNearestSymbol) {
  root.addSymbol(new nth::Identifier("foo"));
  nth::SymbolTable *table = root.beget();
  table->addSymbol(new nth::Identifier("foo"));
  nth::Symbol *symbol = table->
  findSymbol(new nth::Identifier("foo"));
  nth::Symbol *top = table->getSymbols().back();
  EXPECT_EQ(top, symbol);
}

TEST_F(SymbolTableTest, FindSymbolInParentScope) {
  root.addSymbol(new nth::Identifier("foo"));
  nth::SymbolTable *table = root.beget();
  table->addSymbol(new nth::Identifier("bar"));

  nth::Symbol *symbol = table->findSymbol(new nth::Identifier("foo"));
  nth::Symbol *top = root.getSymbols().front();
  EXPECT_EQ(top, symbol);
}

TEST_F(SymbolTableTest, findSymbolReturnsNullIfNotFound) {
  nth::Symbol *not_found = root.findSymbol(new nth::Identifier("unknown"));
  EXPECT_EQ(nullptr, not_found);
}
