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
}

TEST_F(DriverTest, ParseSomeInts) {
  int status = d.parseString("10\n20\n30\n40\n");
  EXPECT_EQ(0, status);
  EXPECT_EQ(4, d.result.getExpressions().size());
}

TEST_F(DriverTest, ParseNothing) {
  nth::Driver d;
  int status = d.parseString("");
  EXPECT_EQ(1, status);
}
