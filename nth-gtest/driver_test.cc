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

TEST(DriverTest, ParseNth) {
  nth::Driver d;
  int status = d.parse(getResourcePath() + "/nth.nth");
  EXPECT_EQ(0, status);
  EXPECT_EQ(nullptr, d.result);
}