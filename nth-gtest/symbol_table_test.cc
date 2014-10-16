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

TEST_F(SymbolTableTest, AddsFirstFrameIfMissing) {
  st.addSymbol(new nth::Identifier("foo"));
  EXPECT_EQ(1, st.getFramesizes().size());
}

TEST_F(SymbolTableTest, IncrementsFramesize) {
  st.addSymbol(new nth::Identifier("foo"));
  st.addSymbol(new nth::Identifier("bar"));
  EXPECT_EQ(1, st.getFramesizes().size());
  EXPECT_EQ(2, st.getFramesizes().top());
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

TEST_F(SymbolTableTest, popFrameRemovesCorrectSymbols) {
  st.addSymbol(new nth::Identifier("foo"));
  st.addSymbol(new nth::Identifier("bar"));
  st.pushFrame();
  st.addSymbol(new nth::Identifier("baz"));
  st.popFrame();

  auto symbols = st.getSymbols();
  auto framesizes = st.getFramesizes();
  EXPECT_EQ(2, symbols.size());
  EXPECT_EQ(1, framesizes.size());
  EXPECT_EQ(2, framesizes.top());
  EXPECT_STREQ("foo", *symbols[0]);
  EXPECT_STREQ("bar", *symbols[1]);
}

TEST_F(SymbolTableTest, pushFrame) {
  st.addSymbol(new nth::Identifier("foo"));
  st.pushFrame();
  EXPECT_EQ(2, st.getFramesizes().size());
  EXPECT_EQ(0, st.getFramesizes().top());
}

TEST_F(SymbolTableTest, throwsErrorOnEmptyStack) {
  EXPECT_THROW(st.popFrame(), std::runtime_error);
}

// TODO:
// test that addSymbol invokes checkSymbol
// test that addSymbol sets symbol on identifier
