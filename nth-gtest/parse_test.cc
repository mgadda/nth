#include <gtest/gtest.h>
#include "test_helper.h"
#include "ast_string_printer.h"
#include "driver.h"

class ParseTest : public ::testing::Test {
 protected:
  nth::Driver d;
  AstStringPrinter printer;
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

  EXPECT_AST(
    block(
      integer(10),
      integer(20),
      integer(30),
      integer(40)));
}

TEST_F(ParseTest, ParseHex) {
  d.parseString("0xbaddcafe");
  EXPECT_AST(block(integer(3135097598)));
}

TEST_F(ParseTest, ParseBinary) {
  d.parseString("0b101010");
  EXPECT_AST(block(integer(42)));
}

TEST_F(ParseTest, ParseOctal) {
  d.parseString("010");
  EXPECT_AST(block(integer(8)));
}

TEST_F(ParseTest, ParseSomeFloats) {
  int status = d.parseString("10.2340982\n2.234e-3");
  EXPECT_EQ(0, status);
  EXPECT_EQ(2, d.result->getNodes().size());
  EXPECT_AST(block(float(10.2340982), float(0.002234)));
}

TEST_F(ParseTest, ParseSomeStrings) {
  int status = d.parseString("\"Hello,\\n\\\"World\\\"!\"   \n \"World: Hello!\"");

  EXPECT_EQ(0, status);
  EXPECT_EQ(2, d.result->getNodes().size());

  d.result->accept(printer);

  std::string expectedValue = "block(string(Hello,\\n\\\"World\\\"!),string(World: Hello!))";
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

  EXPECT_AST(block(boolean(true), boolean(false)));
}

TEST_F(ParseTest, ParseIdentifier) {
  int status = d.parseString("a");
  EXPECT_EQ(0, status);
  EXPECT_EQ(1, d.result->getNodes().size());

  EXPECT_AST(block(ident(a)));
};

TEST_F(ParseTest, ParseArray) {
  int status = d.parseString("[1,2,3]");
  EXPECT_EQ(0, status);
  EXPECT_EQ(1, d.result->getNodes().size());

  EXPECT_AST(block(array(integer(1), integer(2), integer(3))));
}

TEST_F(ParseTest, ParseEmptyArray) {
  int status = d.parseString("[]");
  EXPECT_EQ(0, status);
  EXPECT_EQ(1, d.result->getNodes().size());

  EXPECT_AST(block(array()));
}

TEST_F(ParseTest, ParseMap) {
  int status = d.parseString("{\"foo\": 10.342,\n\"bar\": 12.34}");
  EXPECT_EQ(0, status);
  EXPECT_EQ(1, d.result->getNodes().size());

  EXPECT_AST(block(map(string(foo): float(10.342), string(bar): float(12.34))));
}

TEST_F(ParseTest, ParseMapSubscript) {
  d.parseString("h[\"key1\"]");
  EXPECT_AST(block(subscript(ident(h), string(key1))));
}

TEST_F(ParseTest, ParseEmptyMap) {
  int status = d.parseString("{}");
  EXPECT_EQ(0, status);
  EXPECT_EQ(1, d.result->getNodes().size());

  EXPECT_AST(block(map()));
}

TEST_F(ParseTest, ParseAdd) {
  int status = d.parseString("1 + 2 + 3");
  EXPECT_EQ(0, status);
  EXPECT_EQ(1, d.result->getNodes().size());

  EXPECT_AST(
    block(
      add(
        add(
          integer(1),
          integer(2)),
        integer(3))));
}

TEST_F(ParseTest, ParseSubtract) {
  int status = d.parseString("1 - 2 - 3");
  EXPECT_EQ(0, status);
  EXPECT_EQ(1, d.result->getNodes().size());

  EXPECT_AST(block(subtract(subtract(integer(1), integer(2)), integer(3))));
}

TEST_F(ParseTest, ParseMultiply) {
  int status = d.parseString("1 * 2 * 3");
  EXPECT_EQ(0, status);
  EXPECT_EQ(1, d.result->getNodes().size());

  EXPECT_AST(block(multiply(multiply(integer(1), integer(2)), integer(3))));
}

TEST_F(ParseTest, ParseDivide) {
  int status = d.parseString("1 / 2 / 3");
  EXPECT_EQ(0, status);
  EXPECT_EQ(1, d.result->getNodes().size());

  EXPECT_AST(block(divide(divide(integer(1), integer(2)), integer(3))));
}

TEST_F(ParseTest, ParseExponentiate) {
  int status = d.parseString("1 ^ 2 ^ 3");
  EXPECT_EQ(0, status);
  EXPECT_EQ(1, d.result->getNodes().size());

  EXPECT_AST(block(exp(exp(integer(1), integer(2)), integer(3))));
}

TEST_F(ParseTest, ParseModulo) {
  int status = d.parseString("1 % 2 % 3");
  EXPECT_EQ(0, status);
  EXPECT_EQ(1, d.result->getNodes().size());

  EXPECT_AST(block(mod(mod(integer(1), integer(2)), integer(3))));
}

TEST_F(ParseTest, AstPrinter) {
  d.parseString("1 + 2 + 3");
  EXPECT_AST(block(add(add(integer(1), integer(2)), integer(3))));
}

TEST_F(ParseTest, ParseBitShiftLeft) {
  d.parseString("0xff00 << 2");
  EXPECT_AST(block(bitshiftleft(integer(65280), integer(2))));
}

TEST_F(ParseTest, ParseBitShiftRight) {
  d.parseString("0xbadf00d >> 3");
  EXPECT_AST(block(bitshiftright(integer(195948557), integer(3))));
}

TEST_F(ParseTest, ParseBitwiseOr) {
  d.parseString("0xf0 | 0x0f");
  EXPECT_AST(block(bitwiseor(integer(240), integer(15))));
}

TEST_F(ParseTest, ParseBitwiseAnd) {
  d.parseString("0x3e & 0xff");
  EXPECT_AST(block(bitwiseand(integer(62), integer(255))));
}

TEST_F(ParseTest, ParseBitwiseNot) {
  d.parseString("~0b101010");
  EXPECT_AST(block(bitwisenot(integer(42))));
}

TEST_F(ParseTest, ParseLogicalOr) {
  d.parseString("truth || fiction");
  EXPECT_AST(block(logicalor(ident(truth), ident(fiction))));
}

TEST_F(ParseTest, ParseLogicalAnd) {
  d.parseString("is_valid && is_defined");
  EXPECT_AST(block(logicaland(ident(is_valid), ident(is_defined))));
}

TEST_F(ParseTest, ParseLogicalNot) {
  d.parseString("!is_defined");
  EXPECT_AST(block(logicalnot(ident(is_defined))));
}

TEST_F(ParseTest, ParseExclusiveRange) {
  d.parseString("1..10");
  EXPECT_AST(block(range(integer(1), integer(10), exclusive)));
}

TEST_F(ParseTest, ParseInclusiveRange) {
  d.parseString("-3...3");
  EXPECT_AST(block(range(integer(-3), integer(3), inclusive)));
}

TEST_F(ParseTest, ParseTuple) {
  d.parseString("(\"name\", 3, foo)");
  EXPECT_AST(block(tuple(string(name), integer(3), ident(foo))));
}

TEST_F(ParseTest, ParseTupleFieldAccess) {
  d.parseString("(\"name\", 3).1");
  EXPECT_AST(block(fieldaccess(tuple(string(name), integer(3)), integer(1))));
}

TEST_F(ParseTest, ParseEquality) {
  d.parseString("min_val == 2.3252");
  EXPECT_AST(block(equality(ident(min_val), float(2.3252))));
}

TEST_F(ParseTest, ParseInequality) {
  d.parseString("cond != true");
  EXPECT_AST(block(inequality(ident(cond), boolean(true))));
}

TEST_F(ParseTest, ParseLessThan) {
  d.parseString("1.00005 < 1.0001");
  EXPECT_AST(block(lessthan(float(1.00005), float(1.0001))));
}

TEST_F(ParseTest, ParseGreaterThan) {
  d.parseString("4.3 > 3.2");
  EXPECT_AST(block(greaterthan(float(4.3), float(3.2))));
}

TEST_F(ParseTest, ParseLessThanOrEqualTo) {
  d.parseString("3 <= 4");
  EXPECT_AST(block(lessthanorequalto(integer(3), integer(4))));
}

TEST_F(ParseTest, ParseGreaterThanOrEqualTo) {
  d.parseString("6 >= 4");
  EXPECT_AST(block(greaterthanorequalto(integer(6), integer(4))));
}

TEST_F(ParseTest, ParseParenthesis) {
  d.parseString("3 * (4 + 5)");
  EXPECT_AST(block(multiply(integer(3), add(integer(4), integer(5)))));
}

TEST_F(ParseTest, ParseFunctionDefinition) {
  d.parseString(
    "def add(a: Integer, b: (String, Integer)): Integer {\n\
       a + b.1\n\
    }");

  EXPECT_AST(
    block(
      funcdef(
        name(ident(add)),
        arglist(
          argument(ident(a), simple_type(ident(Integer))),
          argument(
            ident(b),
            templated_type(
              ident(Tuple2),
              simple_type(ident(String)),
              simple_type(ident(Integer))))),
        returning(simple_type(ident(Integer))),
        block(add(ident(a), fieldaccess(ident(b), integer(1)))))));
}

TEST_F(ParseTest, ParseFunctionDefWithoutArguments) {
  d.parseString("def getName(): String { \"Matt\" }");

  EXPECT_AST(
    block(
      funcdef(
        name(ident(getName)),
        arglist(),
        returning(simple_type(ident(String))),
        block(string(Matt)))));
}

TEST_F(ParseTest, ParseVariableDef) {
 d.parseString("val a: Boolean = true");
 EXPECT_AST(block(variabledef(name(ident(a)), simple_type(ident(Boolean)), boolean(true))));
}

TEST_F(ParseTest, ParseFunctionCall) {
  d.parseString("foo(10, 3 + 5)");
  EXPECT_AST(block(call(ident(foo), arguments(integer(10), add(integer(3), integer(5))))));
}

TEST_F(ParseTest, ParseFunctionCallWithoutArguments) {
  d.parseString("argless()");
  EXPECT_AST(block(call(ident(argless), arguments())));
}

TEST_F(ParseTest, ParseIfElse) {
  d.parseString("if (done) { doSomething() } else { doSomethingElse() }");

  EXPECT_AST(
    block(
      ifelse(
        ident(done),
        block(call(ident(doSomething), arguments())),
        block(call(ident(doSomethingElse), arguments())))));
}

TEST_F(ParseTest, ParseLambdaDef) {
  d.parseString("(a: Int): Int => a * 2");
  EXPECT_AST(
    block(
      lambda(
        arglist(argument(ident(a), simple_type(ident(Int)))),
        returning(simple_type(ident(Int))),
        multiply(ident(a), integer(2)))));
}

TEST_F(ParseTest, ParseLambdaAsReturnType) {
  d.parseString("def makeAdder(a: Int): (Int) => Int { (x: Int): Int => x * a }");

  EXPECT_AST(
    block(
      funcdef(
        name(ident(makeAdder)),
        arglist(argument(ident(a), simple_type(ident(Int)))),
        returning(
          templated_type(
            ident(Function1),
            simple_type(ident(Int)),
            simple_type(ident(Int)))),
        block(
          lambda(
            arglist(
              argument(
                ident(x),
                simple_type(ident(Int)))),
            returning(simple_type(ident(Int))),
            multiply(ident(x), ident(a)))))));
}

TEST_F(ParseTest, ParseLambdaAsArgument) {
  d.parseString("def doWork(callbackFun: () => Int): Int { callbackFun() }");

  EXPECT_AST(
    block(
      funcdef(
        name(ident(doWork)),
        arglist(
          argument(
            ident(callbackFun),
            templated_type(
              ident(Function0),
              simple_type(ident(Int))))),
        returning(simple_type(ident(Int))),
        block(
          call(
            ident(callbackFun),
               arguments()))))
  );
}

TEST_F(ParseTest, ParseFunctionDefWithTypeParameters) {
 d.parseString("def drop1[T](list: List[T]): List[T] { list.tail } ");

 EXPECT_AST(
   block(
     funcdef(
       name(ident(drop1)),
       arglist(
         argument(ident(list), templated_type(ident(List), simple_type(ident(T))))),
       returning(templated_type(ident(List), simple_type(ident(T)))),
             block(fieldaccess(ident(list), ident(tail))))));
}

TEST_F(ParseTest, ParseTypeAliasDefStatements) {
  d.parseString("type ExpressionList = List[Expression]");

  EXPECT_AST(
    block(type_alias(
      simple_type(ident(ExpressionList)),
      templated_type(
        ident(List),
        simple_type(ident(Expression))))));
}
