#include <gtest/gtest.h>
#include "test_helper.h"
#include "ast_printer.h"
#include "driver.h"

class ParseTest : public ::testing::Test {
 protected:
  nth::Driver d;
  AstPrinter printer;
  virtual void SetUp() {}
};

// Sanity check that we can still parse the nth "spec"
// TODO: replace this with something akin to LLVM's lit (or lit itself)
TEST_F(ParseTest, ParseNth) {
  int status = d.parse(getResourcePath() + "/nth.nth");
  EXPECT_EQ(0, status);
  EXPECT_NE(nullptr, d.result);
}

TEST_F(ParseTest, ParseNothing) {
  int status = d.parseString("");
  EXPECT_EQ(1, status);
}

TEST_F(ParseTest, ParseSomeInts) {
  int status = d.parseString("10\n20\n30\n40\n");
  EXPECT_EQ(0, status);
  EXPECT_EQ(4, d.result->getNodes().size());

  d.result->accept(printer);

  EXPECT_STREQ("block(integer(10), integer(20), integer(30), integer(40))", printer.getOutput().c_str());
}

TEST_F(ParseTest, ParseHex) {
  d.parseString("0xbaddcafe");
  d.result->accept(printer);
  EXPECT_STREQ("block(integer(3135097598))", printer.getOutput().c_str());
}

TEST_F(ParseTest, ParseBinary) {
  d.parseString("0b101010");
  d.result->accept(printer);
  EXPECT_STREQ("block(integer(42))", printer.getOutput().c_str());
}

TEST_F(ParseTest, ParseOctal) {
  d.parseString("010");
  d.result->accept(printer);
  EXPECT_STREQ("block(integer(8))", printer.getOutput().c_str());
}

TEST_F(ParseTest, ParseSomeFloats) {
  int status = d.parseString("10.2340982\n2.234e-3");
  EXPECT_EQ(0, status);
  EXPECT_EQ(2, d.result->getNodes().size());

  d.result->accept(printer);

  EXPECT_STREQ("block(float(10.2340982), float(0.002234))", printer.getOutput().c_str());
}

TEST_F(ParseTest, ParseSomeStrings) {
  int status = d.parseString("\"Hello,\\n\\\"World\\\"!\"   \n \"World: Hello!\"");

  EXPECT_EQ(0, status);
  EXPECT_EQ(2, d.result->getNodes().size());

  d.result->accept(printer);

  std::string expectedValue = "block(string(Hello,\\n\\\"World\\\"!), string(World: Hello!))";
  EXPECT_STREQ(expectedValue.c_str(), printer.getOutput().c_str());
}

TEST_F(ParseTest, ParseEscapedEscapeCharacters) {
  d.parseString(R"("a\\b")");
  d.result->accept(printer);
  EXPECT_STREQ(R"*(block(string(a\\b)))*", printer.getOutput().c_str());
}

TEST_F(ParseTest, ParseSomeBools) {
  int status = d.parseString("true\nfalse");
  EXPECT_EQ(0, status);
  EXPECT_EQ(2, d.result->getNodes().size());

  d.result->accept(printer);

  EXPECT_STREQ("block(boolean(true), boolean(false))", printer.getOutput().c_str());
}

TEST_F(ParseTest, ParseIdentifier) {
  int status = d.parseString("a");
  EXPECT_EQ(0, status);
  EXPECT_EQ(1, d.result->getNodes().size());

  d.result->accept(printer);

  EXPECT_STREQ("block(ident(a))", printer.getOutput().c_str());
};

TEST_F(ParseTest, ParseArray) {
  int status = d.parseString("[1,2,3]");
  EXPECT_EQ(0, status);
  EXPECT_EQ(1, d.result->getNodes().size());

  d.result->getNodes()[0]->accept(printer);

  EXPECT_STREQ("array(integer(1), integer(2), integer(3))", printer.getOutput().c_str());
}

TEST_F(ParseTest, ParseEmptyArray) {
  int status = d.parseString("[]");
  EXPECT_EQ(0, status);
  EXPECT_EQ(1, d.result->getNodes().size());

  d.result->getNodes()[0]->accept(printer);

  EXPECT_STREQ("array()", printer.getOutput().c_str());
}

TEST_F(ParseTest, ParseMap) {
  int status = d.parseString("{\"foo\": 10.342,\n\"bar\": 12.34}");
  EXPECT_EQ(0, status);
  EXPECT_EQ(1, d.result->getNodes().size());

  d.result->getNodes()[0]->accept(printer);

  EXPECT_STREQ("map(string(foo): float(10.342), string(bar): float(12.34))", printer.getOutput().c_str());
}

TEST_F(ParseTest, ParseMapSubscript) {
  d.parseString("h[\"key1\"]");
  d.result->accept(printer);

  EXPECT_STREQ("block(subscript(ident(h), string(key1)))", printer.getOutput().c_str());
}

TEST_F(ParseTest, ParseEmptyMap) {
  int status = d.parseString("{}");
  EXPECT_EQ(0, status);
  EXPECT_EQ(1, d.result->getNodes().size());

  d.result->getNodes()[0]->accept(printer);

  EXPECT_STREQ("map()", printer.getOutput().c_str());
}

TEST_F(ParseTest, ParseAdd) {
  int status = d.parseString("1 + 2 + 3");
  EXPECT_EQ(0, status);
  EXPECT_EQ(1, d.result->getNodes().size());

  d.result->getNodes()[0]->accept(printer);

  EXPECT_STREQ("add(add(integer(1), integer(2)), integer(3))", printer.getOutput().c_str());
}

TEST_F(ParseTest, ParseSubtract) {
  int status = d.parseString("1 - 2 - 3");
  EXPECT_EQ(0, status);
  EXPECT_EQ(1, d.result->getNodes().size());

  d.result->getNodes()[0]->accept(printer);

  EXPECT_STREQ("subtract(subtract(integer(1), integer(2)), integer(3))", printer.getOutput().c_str());
}

TEST_F(ParseTest, ParseMultiply) {
  int status = d.parseString("1 * 2 * 3");
  EXPECT_EQ(0, status);
  EXPECT_EQ(1, d.result->getNodes().size());

  d.result->getNodes()[0]->accept(printer);

  EXPECT_STREQ("multiply(multiply(integer(1), integer(2)), integer(3))", printer.getOutput().c_str());
}

TEST_F(ParseTest, ParseDivide) {
  int status = d.parseString("1 / 2 / 3");
  EXPECT_EQ(0, status);
  EXPECT_EQ(1, d.result->getNodes().size());

  d.result->getNodes()[0]->accept(printer);

  EXPECT_STREQ("divide(divide(integer(1), integer(2)), integer(3))", printer.getOutput().c_str());
}

TEST_F(ParseTest, ParseExponentiate) {
  int status = d.parseString("1 ^ 2 ^ 3");
  EXPECT_EQ(0, status);
  EXPECT_EQ(1, d.result->getNodes().size());

  d.result->getNodes()[0]->accept(printer);

  EXPECT_STREQ("exp(exp(integer(1), integer(2)), integer(3))", printer.getOutput().c_str());
}

TEST_F(ParseTest, ParseModulo) {
  int status = d.parseString("1 % 2 % 3");
  EXPECT_EQ(0, status);
  EXPECT_EQ(1, d.result->getNodes().size());

  d.result->getNodes()[0]->accept(printer);

  EXPECT_STREQ("mod(mod(integer(1), integer(2)), integer(3))", printer.getOutput().c_str());
}

TEST_F(ParseTest, AstPrinter) {
  d.parseString("1 + 2 + 3");
  d.result->getNodes()[0]->accept(printer);
  EXPECT_STREQ("add(add(integer(1), integer(2)), integer(3))", printer.getOutput().c_str());
}

TEST_F(ParseTest, ParseBitShiftLeft) {
  d.parseString("0xff00 << 2");
  d.result->getNodes()[0]->accept(printer);
  EXPECT_STREQ("bitshiftleft(integer(65280), integer(2))", printer.getOutput().c_str());
}

TEST_F(ParseTest, ParseBitShiftRight) {
  d.parseString("0xbadf00d >> 3");
  d.result->getNodes()[0]->accept(printer);
  EXPECT_STREQ("bitshiftright(integer(195948557), integer(3))", printer.getOutput().c_str());
}

TEST_F(ParseTest, ParseBitwiseOr) {
  d.parseString("0xf0 | 0x0f");
  d.result->getNodes()[0]->accept(printer);
  EXPECT_STREQ("bitwiseor(integer(240), integer(15))", printer.getOutput().c_str());
}

TEST_F(ParseTest, ParseBitwiseAnd) {
  d.parseString("0x3e & 0xff");
  d.result->getNodes()[0]->accept(printer);
  EXPECT_STREQ("bitwiseand(integer(62), integer(255))", printer.getOutput().c_str());
}

TEST_F(ParseTest, ParseBitwiseNot) {
  d.parseString("~0b101010");
  d.result->getNodes()[0]->accept(printer);
  EXPECT_STREQ("bitwisenot(integer(42))", printer.getOutput().c_str());
}

TEST_F(ParseTest, ParseLogicalOr) {
  d.parseString("truth || fiction");
  d.result->getNodes()[0]->accept(printer);
  EXPECT_STREQ("logicalor(ident(truth), ident(fiction))", printer.getOutput().c_str());
}

TEST_F(ParseTest, ParseLogicalAnd) {
  d.parseString("is_valid && is_defined");
  d.result->getNodes()[0]->accept(printer);
  EXPECT_STREQ("logicaland(ident(is_valid), ident(is_defined))", printer.getOutput().c_str());
}

TEST_F(ParseTest, ParseLogicalNot) {
  d.parseString("!is_defined");
  d.result->accept(printer);
  EXPECT_STREQ("block(logicalnot(ident(is_defined)))", printer.getOutput().c_str());
}

TEST_F(ParseTest, ParseExclusiveRange) {
  d.parseString("1..10");
  d.result->accept(printer);
  EXPECT_STREQ("block(range(integer(1), integer(10), exclusive))", printer.getOutput().c_str());
}

TEST_F(ParseTest, ParseInclusiveRange) {
  d.parseString("-3...3");
  d.result->accept(printer);
  EXPECT_STREQ("block(range(integer(-3), integer(3), inclusive))", printer.getOutput().c_str());
}

TEST_F(ParseTest, ParseTuple) {
  d.parseString("(\"name\", 3, foo)");
  d.result->accept(printer);
  EXPECT_STREQ("block(tuple(string(name), integer(3), ident(foo)))", printer.getOutput().c_str());
}

TEST_F(ParseTest, ParseEmptyTuple) {
  d.parseString("()");
  d.result->accept(printer);
  EXPECT_STREQ("block(tuple())", printer.getOutput().c_str());
}

TEST_F(ParseTest, ParseTupleFieldAccess) {
  d.parseString("(\"name\", 3).1");
  d.result->accept(printer);
  EXPECT_STREQ("block(tuplefieldaccess(tuple(string(name), integer(3)), integer(1)))", printer.getOutput().c_str());
}

TEST_F(ParseTest, ParseEquality) {
  d.parseString("min_val == 2.3252");
  d.result->accept(printer);
  EXPECT_STREQ("block(equality(ident(min_val), float(2.3252)))", printer.getOutput().c_str());
}

TEST_F(ParseTest, ParseInequality) {
  d.parseString("cond != true");
  d.result->accept(printer);
  EXPECT_STREQ("block(inequality(ident(cond), boolean(true)))", printer.getOutput().c_str());
}

TEST_F(ParseTest, ParseLessThan) {
  d.parseString("1.00005 < 1.0001");
  d.result->accept(printer);
  EXPECT_STREQ("block(lessthan(float(1.00005), float(1.0001)))", printer.getOutput().c_str());
}

TEST_F(ParseTest, ParseGreaterThan) {
  d.parseString("4.3 > 3.2");
  d.result->accept(printer);
  EXPECT_STREQ("block(greaterthan(float(4.3), float(3.2)))", printer.getOutput().c_str());
}

TEST_F(ParseTest, ParseLessThanOrEqualTo) {
  d.parseString("3 <= 4");
  d.result->accept(printer);
  EXPECT_STREQ("block(lessthanorequalto(integer(3), integer(4)))", printer.getOutput().c_str());
}

TEST_F(ParseTest, ParseGreaterThanOrEqualTo) {
  d.parseString("6 >= 4");
  d.result->accept(printer);
  EXPECT_STREQ("block(greaterthanorequalto(integer(6), integer(4)))", printer.getOutput().c_str());
}

TEST_F(ParseTest, ParseParenthesis) {
  d.parseString("3 * (4 + 5)");
  d.result->accept(printer);
  EXPECT_STREQ("block(multiply(integer(3), add(integer(4), integer(5))))", printer.getOutput().c_str());
}

TEST_F(ParseTest, ParseFunctionDefinition) {
  d.parseString("def add(a: Integer, b: (String, Integer)): Integer {\n\
                   a + b.1\n\
                 }");
  d.result->accept(printer);
  EXPECT_STREQ("block(funcdef(name(ident(add)), arglist(argument(ident(a), simple_type(ident(Integer))), argument(ident(b), templated_type(ident(Tuple2), simple_type(ident(String)), simple_type(ident(Integer))))), returning(simple_type(ident(Integer))), block(add(ident(a), tuplefieldaccess(ident(b), integer(1))))))", printer.getOutput().c_str());
}

TEST_F(ParseTest, ParseVariableDef) {
 d.parseString("val a: Boolean = true");
 d.result->accept(printer);
 EXPECT_STREQ("block(variabledef(name(ident(a)), simple_type(ident(Boolean)), boolean(true)))", printer.getOutput().c_str());
}
