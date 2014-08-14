//
//  objc_helper.h
//  nth
//
//  Created by Matt Gadda on 7/13/14.
//  Copyright (c) 2014 Matt Gadda. All rights reserved.
//

#include <string>
#include <sstream>
#include <regex>

#include "ast_visitor.h"


std::string getResourcePath();
std::string stripMargin(std::string str);

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

