#include <gtest/gtest.h>
#include "test_helper.h"
#include "driver.h"
#include "scope_checker.h"

class ScopeCheckerTest : public ::testing::Test {
 protected:
  nth::Driver d;
  nth::ScopeChecker scopeChecker;
  virtual void SetUp() {
  }
};

TEST_F(ScopeCheckerTest, TestValidFunctionDef) {
  d.parseString("def appendUsToTs[T, U](a: T, b: U): T { a + b }");
  scopeChecker.run(d.result);
  EXPECT_EQ(true, scopeChecker.isValid());
}

TEST_F(ScopeCheckerTest, TestInvalidFunctionDef) {
  d.parseString("def appendUsToTs[T, U](a: T, b: U): T { c.append(d) }");
  scopeChecker.run(d.result);
  EXPECT_EQ(false, scopeChecker.isValid());
  ASSERT_EQ(2, scopeChecker.getUnknownIdentifiers().size());

  nth::IdentifierList &list = scopeChecker.getUnknownIdentifiers();
  EXPECT_EQ("c", list.front()->getValue());
  EXPECT_EQ("d", list.back()->getValue());
}

TEST_F(ScopeCheckerTest, TestDeclareBeforeUseVals) {
  d.parseString("val a: Int = b * c");
  scopeChecker.run(d.result);
  EXPECT_EQ(false, scopeChecker.isValid());
  ASSERT_EQ(3, scopeChecker.getUnknownIdentifiers().size());

  nth::IdentifierList &list = scopeChecker.getUnknownIdentifiers();
  EXPECT_EQ("Int", list.front()->getValue()); list.pop_front();
  EXPECT_EQ("b", list.front()->getValue()); list.pop_front();
  EXPECT_EQ("c", list.front()->getValue());
}

TEST_F(ScopeCheckerTest, TestDeclareBeforeUseValsValid) {
  d.parseString("val b: Int = 10\nval a: Int = b * 2");
  scopeChecker.run(d.result);
  EXPECT_EQ(false, scopeChecker.isValid());
  ASSERT_EQ(2, scopeChecker.getUnknownIdentifiers().size());

  nth::IdentifierList &list = scopeChecker.getUnknownIdentifiers();
  EXPECT_EQ("Int", list.front()->getValue()); list.pop_front();
  EXPECT_EQ("Int", list.front()->getValue());
}
