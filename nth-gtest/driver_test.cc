//
//  driver_test.cc
//  nth
//
//  Created by Matt Gadda on 7/13/14.
//  Copyright (c) 2014 Matt Gadda. All rights reserved.
//

#include <gtest/gtest.h>
#include "test_helper.h"
#include "driver.h"

// Used by EXPECT_EXPR_EQ below
template<typename T, typename U>
bool compare(U expected, const T &actual) {
  return actual == expected;
}

// TODO: fix this
#define EXPECT_EXPR_EQ(derived_type, expected, expr) \
nth::derived_type *casted = static_cast<nth::derived_type*>(expr); \
nth::derived_type &actual = *casted; \
EXPECT_PRED2(compare, expected, actual);


class DriverTest : public ::testing::Test {
 protected:
  nth::Driver d;
  virtual void SetUp() {}
};

// Sanity check that we can still parse the nth spec
// TODO: replace this with something akin to LLVM's lit (or lit itself)
TEST_F(DriverTest, ParseNth) {
  int status = d.parse(getResourcePath() + "/nth.nth");
  EXPECT_EQ(0, status);
  EXPECT_NE(nullptr, d.result);
}

TEST_F(DriverTest, ParseNothing) {
  int status = d.parseString("");
  EXPECT_EQ(1, status);
}

TEST_F(DriverTest, ParseSomeInts) {
  int status = d.parseString("10\n20\n30\n40\n");
  EXPECT_EQ(0, status);
  EXPECT_EQ(4, d.result->getExpressions().size());
  
  nth::Expression *expr = d.result->getExpressions()[0];
  nth::Integer *i = dynamic_cast<nth::Integer*>(expr);
  EXPECT_EQ(10, *i);
  
  expr = d.result->getExpressions()[1];
  i = static_cast<nth::Integer*>(expr);
  EXPECT_EQ(20, *i);
  
  expr = d.result->getExpressions()[2];
  i = static_cast<nth::Integer*>(expr);
  EXPECT_EQ(30, *i);
  
  expr = d.result->getExpressions()[3];
  i = static_cast<nth::Integer*>(expr);
  EXPECT_EQ(40, *i);
}


TEST_F(DriverTest, ParseSomeFloats) {
  int status = d.parseString("10.2340982\n2.234e-3");
  EXPECT_EQ(0, status);
  EXPECT_EQ(2, d.result->getExpressions().size());

  nth::Expression *expr = d.result->getExpressions()[0];
  nth::Float *f = dynamic_cast<nth::Float*>(expr);
  ASSERT_NE(nullptr, f);
  EXPECT_EQ(10.2340982, *f);

  expr = d.result->getExpressions()[1];
  f = dynamic_cast<nth::Float*>(expr);
  ASSERT_NE(nullptr, f);
  EXPECT_EQ(0.002234, *f);
}

TEST_F(DriverTest, ParseSomeStrings) {
  int status = d.parseString("\"Hello, World!\n\"\n\"World: Hello!\"");
  EXPECT_EQ(0, status);
  EXPECT_EQ(2, d.result->getExpressions().size());

  nth::Expression *expr = d.result->getExpressions()[0];
  nth::String *s = dynamic_cast<nth::String*>(expr);
  ASSERT_NE(nullptr, s);
  EXPECT_STREQ("Hello, World!\n", *s);

  expr = d.result->getExpressions()[1];
  s = dynamic_cast<nth::String*>(expr);
  ASSERT_NE(nullptr, s);
  EXPECT_STREQ("World: Hello!", *s);
}

TEST_F(DriverTest, ParseSomeBools) {
  int status = d.parseString("true\nfalse");
  EXPECT_EQ(0, status);
  EXPECT_EQ(2, d.result->getExpressions().size());

  nth::Expression *expr = d.result->getExpressions()[0];
  nth::Boolean *b = dynamic_cast<nth::Boolean*>(expr);
  ASSERT_NE(nullptr, b);
  EXPECT_EQ(true, *b);

  expr = d.result->getExpressions()[1];
  b = dynamic_cast<nth::Boolean*>(expr);
  ASSERT_NE(nullptr, b);
  EXPECT_EQ(false, *b);
}

TEST_F(DriverTest, ParseIdentifier) {
  int status = d.parseString("a");
  EXPECT_EQ(0, status);
  EXPECT_EQ(1, d.result->getExpressions().size());

  nth::Expression *expr = d.result->getExpressions()[0];
  nth::Identifier *i = dynamic_cast<nth::Identifier*>(expr);
  ASSERT_NE(nullptr, i);
  EXPECT_STREQ("a", *i);
};

TEST_F(DriverTest, ParseArray) {
  int status = d.parseString("[1,2,3]");
  EXPECT_EQ(0, status);
  EXPECT_EQ(1, d.result->getExpressions().size());

  nth::Expression *expr = d.result->getExpressions()[0];
  nth::Array *a = dynamic_cast<nth::Array*>(expr);
  EXPECT_EQ(3, a->getValues().size());

  EXPECT_EQ(1, *dynamic_cast<nth::Integer*>(a->getValues()[0]));
  EXPECT_EQ(2, *dynamic_cast<nth::Integer*>(a->getValues()[1]));
  EXPECT_EQ(3, *dynamic_cast<nth::Integer*>(a->getValues()[2]));
}

TEST_F(DriverTest, ParseMap) {
  int status = d.parseString("{\"foo\": 10.342,\n\"bar\": 12.34}");
  EXPECT_EQ(0, status);
  EXPECT_EQ(1, d.result->getExpressions().size());

  nth::Expression *expr = d.result->getExpressions()[0];
  nth::Map *a = dynamic_cast<nth::Map*>(expr);
  EXPECT_EQ(2, a->getValues().size());

  EXPECT_STREQ("foo", *a->getValues()[0].first);
  EXPECT_EQ(10.342, *dynamic_cast<nth::Float*>(a->getValues()[0].second));

  EXPECT_STREQ("bar", *a->getValues()[1].first);
  EXPECT_EQ(12.34, *dynamic_cast<nth::Float*>(a->getValues()[1].second));
}

TEST_F(DriverTest, ParseAdd) {
  // 1 + 2 + 3 => add(add(1,2), 3)
  int status = d.parseString("1 + 2 + 3");
  EXPECT_EQ(0, status);
  EXPECT_EQ(1, d.result->getExpressions().size());

  nth::Expression *expr = d.result->getExpressions()[0];
  nth::Add *a1 = dynamic_cast<nth::Add*>(expr);
  
  ASSERT_NE(nullptr, a1);

  nth::Expression &e1 = *(a1->left);
  nth::Expression &e2 = *(a1->right);
  
  nth::Add* a2 = dynamic_cast<nth::Add*>(&e1);
  ASSERT_NE(nullptr, a2);
  
  nth::Expression &e3 = *(a2->left);
  nth::Expression &e4 = *(a2->right);

  nth::Integer* i1 = dynamic_cast<nth::Integer*>(&e3);
  nth::Integer* i2 = dynamic_cast<nth::Integer*>(&e4);
  nth::Integer* i3 = dynamic_cast<nth::Integer*>(&e2);
  EXPECT_EQ(1, *i1);
  EXPECT_EQ(2, *i2);
  EXPECT_EQ(3, *i3);
}

TEST_F(DriverTest, ParseMultiply) {
  // 1 + 2 + 3 => Multiply(Multiply(1,2), 3)
  int status = d.parseString("1 * 2 * 3");
  EXPECT_EQ(0, status);
  EXPECT_EQ(1, d.result->getExpressions().size());

  nth::Expression *expr = d.result->getExpressions()[0];
  nth::Multiply *a1 = dynamic_cast<nth::Multiply*>(expr);
  
  ASSERT_NE(nullptr, a1);

  nth::Expression &e1 = *(a1->left);
  nth::Expression &e2 = *(a1->right);
  
  nth::Multiply* a2 = dynamic_cast<nth::Multiply*>(&e1);
  ASSERT_NE(nullptr, a2);
  
  nth::Expression &e3 = *(a2->left);
  nth::Expression &e4 = *(a2->right);

  nth::Integer* i1 = dynamic_cast<nth::Integer*>(&e3);
  nth::Integer* i2 = dynamic_cast<nth::Integer*>(&e4);
  nth::Integer* i3 = dynamic_cast<nth::Integer*>(&e2);
  EXPECT_EQ(1, *i1);
  EXPECT_EQ(2, *i2);
  EXPECT_EQ(3, *i3);
}

TEST_F(DriverTest, ParseDivide) {
  // 1 + 2 + 3 => Divide(Divide(1,2), 3)
  int status = d.parseString("1 / 2 / 3");
  EXPECT_EQ(0, status);
  EXPECT_EQ(1, d.result->getExpressions().size());

  nth::Expression *expr = d.result->getExpressions()[0];
  nth::Divide *a1 = dynamic_cast<nth::Divide*>(expr);
  
  ASSERT_NE(nullptr, a1);

  nth::Expression &e1 = *(a1->left);
  nth::Expression &e2 = *(a1->right);
  
  nth::Divide* a2 = dynamic_cast<nth::Divide*>(&e1);
  ASSERT_NE(nullptr, a2);
  
  nth::Expression &e3 = *(a2->left);
  nth::Expression &e4 = *(a2->right);

  nth::Integer* i1 = dynamic_cast<nth::Integer*>(&e3);
  nth::Integer* i2 = dynamic_cast<nth::Integer*>(&e4);
  nth::Integer* i3 = dynamic_cast<nth::Integer*>(&e2);
  EXPECT_EQ(1, *i1);
  EXPECT_EQ(2, *i2);
  EXPECT_EQ(3, *i3);
}


