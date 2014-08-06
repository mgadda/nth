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
  AstPrinter printer;
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
  
  d.result->accept(printer);
  
  EXPECT_STREQ("block(integer(10), integer(20), integer(30), integer(40))", printer.getOutput().c_str());
}


TEST_F(DriverTest, ParseSomeFloats) {
  int status = d.parseString("10.2340982\n2.234e-3");
  EXPECT_EQ(0, status);
  EXPECT_EQ(2, d.result->getExpressions().size());

  d.result->accept(printer);
  
  EXPECT_STREQ("block(float(10.2340982), float(0.002234))", printer.getOutput().c_str());
}

TEST_F(DriverTest, ParseSomeStrings) {
  int status = d.parseString("\"Hello, World!\n\"\n\"World: Hello!\"");
  EXPECT_EQ(0, status);
  EXPECT_EQ(2, d.result->getExpressions().size());

  d.result->accept(printer);
  
  EXPECT_STREQ("block(string(Hello, World!\n), string(World: Hello!))", printer.getOutput().c_str());
}

TEST_F(DriverTest, ParseSomeBools) {
  int status = d.parseString("true\nfalse");
  EXPECT_EQ(0, status);
  EXPECT_EQ(2, d.result->getExpressions().size());

  d.result->accept(printer);
  
  EXPECT_STREQ("block(true, false)", printer.getOutput().c_str());
}

TEST_F(DriverTest, ParseIdentifier) {
  int status = d.parseString("a");
  EXPECT_EQ(0, status);
  EXPECT_EQ(1, d.result->getExpressions().size());

  d.result->accept(printer);
  
  EXPECT_STREQ("block(ident(a))", printer.getOutput().c_str());
};

TEST_F(DriverTest, ParseArray) {
  int status = d.parseString("[1,2,3]");
  EXPECT_EQ(0, status);
  EXPECT_EQ(1, d.result->getExpressions().size());

  d.result->getExpressions()[0]->accept(printer);

  EXPECT_STREQ("array(integer(1), integer(2), integer(3))", printer.getOutput().c_str()); 
}

TEST_F(DriverTest, ParseMap) {
  int status = d.parseString("{\"foo\": 10.342,\n\"bar\": 12.34}");
  EXPECT_EQ(0, status);
  EXPECT_EQ(1, d.result->getExpressions().size());

  d.result->getExpressions()[0]->accept(printer);

  EXPECT_STREQ("map(string(foo): float(10.342), string(bar): float(12.34))", printer.getOutput().c_str()); 
}

TEST_F(DriverTest, ParseAdd) {
  int status = d.parseString("1 + 2 + 3");
  EXPECT_EQ(0, status);
  EXPECT_EQ(1, d.result->getExpressions().size());
  
  d.result->getExpressions()[0]->accept(printer);

  EXPECT_STREQ("add(add(integer(1), integer(2)), integer(3))", printer.getOutput().c_str()); 
}

TEST_F(DriverTest, ParseSubtract) {
  int status = d.parseString("1 - 2 - 3");
  EXPECT_EQ(0, status);
  EXPECT_EQ(1, d.result->getExpressions().size());

  d.result->getExpressions()[0]->accept(printer);

  EXPECT_STREQ("subtract(subtract(integer(1), integer(2)), integer(3))", printer.getOutput().c_str()); 
}

TEST_F(DriverTest, ParseMultiply) {
  int status = d.parseString("1 * 2 * 3");
  EXPECT_EQ(0, status);
  EXPECT_EQ(1, d.result->getExpressions().size());

  d.result->getExpressions()[0]->accept(printer);

  EXPECT_STREQ("multiply(multiply(integer(1), integer(2)), integer(3))", printer.getOutput().c_str()); 
}

TEST_F(DriverTest, ParseDivide) {
  int status = d.parseString("1 / 2 / 3");
  EXPECT_EQ(0, status);
  EXPECT_EQ(1, d.result->getExpressions().size());

  d.result->getExpressions()[0]->accept(printer);

  EXPECT_STREQ("divide(divide(integer(1), integer(2)), integer(3))", printer.getOutput().c_str()); 
}

TEST_F(DriverTest, ParseExponentiate) {
  int status = d.parseString("1 ^ 2 ^ 3");
  EXPECT_EQ(0, status);
  EXPECT_EQ(1, d.result->getExpressions().size());

  d.result->getExpressions()[0]->accept(printer);

  EXPECT_STREQ("exp(exp(integer(1), integer(2)), integer(3))", printer.getOutput().c_str()); 
}

TEST_F(DriverTest, ParseModulo) {
  int status = d.parseString("1 % 2 % 3");
  EXPECT_EQ(0, status);
  EXPECT_EQ(1, d.result->getExpressions().size());

  d.result->getExpressions()[0]->accept(printer);

  EXPECT_STREQ("mod(mod(integer(1), integer(2)), integer(3))", printer.getOutput().c_str()); 
}

TEST_F(DriverTest, AstPrinter) {
  d.parseString("1 + 2 + 3");
  d.result->getExpressions()[0]->accept(printer);
  EXPECT_STREQ("add(add(integer(1), integer(2)), integer(3))", printer.getOutput().c_str());
}

TEST_F(DriverTest, ParseBitShiftLeft) {
  d.parseString("0xff00 << 2");
  d.result->getExpressions()[0]->accept(printer);
  EXPECT_STREQ("bitshiftleft(integer(65280), integer(2))", printer.getOutput().c_str());
}

TEST_F(DriverTest, ParseBitShiftRight) {
  d.parseString("0xbadf00d >> 3");
  d.result->getExpressions()[0]->accept(printer);
  EXPECT_STREQ("bitshiftright(integer(195948557), integer(3))", printer.getOutput().c_str());
}

TEST_F(DriverTest, ParseBitwiseOr) {
  d.parseString("0xf0 | 0x0f");
  d.result->getExpressions()[0]->accept(printer);
  EXPECT_STREQ("bitwiseor(integer(240), integer(15))", printer.getOutput().c_str());
}

TEST_F(DriverTest, ParseBitwiseAnd) {
  d.parseString("0x3e & 0xff");
  d.result->getExpressions()[0]->accept(printer);
  EXPECT_STREQ("bitwiseand(integer(62), integer(255))", printer.getOutput().c_str());
}

TEST_F(DriverTest, ParseLogicalOr) {
  d.parseString("truth || fiction");
  d.result->getExpressions()[0]->accept(printer);
  EXPECT_STREQ("logicalor(ident(truth), ident(fiction))", printer.getOutput().c_str());
}

TEST_F(DriverTest, ParseLogicalAnd) {
  d.parseString("is_valid && is_defined");
  d.result->getExpressions()[0]->accept(printer);
  EXPECT_STREQ("logicaland(ident(is_valid), ident(is_defined))", printer.getOutput().c_str());
}

TEST_F(DriverTest, ParseLogicalNot) {
  d.parseString("!is_defined");
  d.result->accept(printer);
  EXPECT_STREQ("block(logicalnot(ident(is_defined)))", printer.getOutput().c_str());
}

