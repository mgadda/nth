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
  // tests that a function works the way we expect it to
  // because a function definition is the most complicated
  d.parseString("def appendUsToTs[T, U](a: T, b: U): T { a + b }");
  d.result->accept(scopeChecker);
  EXPECT_EQ(true, scopeChecker.isValid());
}

TEST_F(ScopeCheckerTest, TestInvalidFunctionDef) {
  d.parseString("def appendUsToTs[T, U](a: T, b: U): T { c.append(d) }");
  d.result->accept(scopeChecker);
  EXPECT_EQ(false, scopeChecker.isValid());
  EXPECT_EQ(3, scopeChecker.getUnknownIdentifiers().size());
}

