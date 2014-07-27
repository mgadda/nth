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

TEST_F(DriverTest, ParseNothing) {
  nth::Driver d;
  int status = d.parseString("");
  EXPECT_EQ(1, status);
}
