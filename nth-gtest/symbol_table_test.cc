#include <stdexcept>
#include <gtest/gtest.h>
#include "test_helper.h"
#include "symbol_table.h"
#include <stack>

namespace nth {
  class Symbol;
}

class SymbolTableTest : public ::testing::Test {

protected:
  nth::SymbolTable st;
  virtual void SetUp() {}
};

TEST_F(SymbolTableTest, AddsFirstScopeIfMissing) {
  st.addSymbol(new nth::Identifier("foo"));
  EXPECT_EQ(1, st.getScopes().size());
}

TEST_F(SymbolTableTest, FindsNearestSymbol) {
  st.addSymbol(new nth::Identifier("foo"));
  st.addSymbol(new nth::Identifier("bar"));
  nth::Symbol *symbol = st.findSymbol(new nth::Identifier("foo"));
  nth::Symbol *top = st.getSymbols()[0];
  EXPECT_EQ(top, symbol);
}

TEST_F(SymbolTableTest, findSymbolReturnsNullIfNotFound) {
  nth::Symbol *not_found = st.findSymbol(new nth::Identifier("unknown"));
  EXPECT_EQ(nullptr, not_found);
}

TEST_F(SymbolTableTest, popScopeRemovesCorrectSymbols) {
  st.addSymbol(new nth::Identifier("foo"));
  st.addSymbol(new nth::Identifier("bar"));
  st.pushScope();
  st.addSymbol(new nth::Identifier("baz"));
  st.popScope();

  auto symbols = st.getSymbols();
  auto scopes = st.getScopes();
  EXPECT_EQ(2, symbols.size());
  EXPECT_EQ(1, scopes.size());
  EXPECT_EQ(2, scopes.back().size());
  EXPECT_STREQ("foo", *symbols[0]);
  EXPECT_STREQ("bar", *symbols[1]);
}

TEST_F(SymbolTableTest, pushScope) {
  st.addSymbol(new nth::Identifier("foo"));
  st.pushScope();
  EXPECT_EQ(2, st.getScopes().size());
  EXPECT_EQ(0, st.getScopes().back().size());
}

TEST_F(SymbolTableTest, throwsErrorOnEmptyStack) {
  EXPECT_THROW(st.popScope(), std::runtime_error);
}

// TODO:
// test that addSymbol invokes checkSymbol
// test that addSymbol sets symbol on identifier
