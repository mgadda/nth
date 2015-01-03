#include <gtest/gtest.h>
#include "test_helper.h"
#include "driver.h"
#include "scope_checker.h"

class ScopeCheckerTest : public ::testing::Test {
 public:
  // TODO: fix this horrific shit
  ScopeCheckerTest() : scopeChecker(*(new nth::ScopeChecker( *(new nth::SymbolTable( {new nth::Identifier("Int"), new nth::Identifier("Float")} ) ) ) )) {}
 protected:
  nth::Driver d;
  nth::ScopeChecker &scopeChecker;

  virtual void SetUp() {}
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

TEST_F(ScopeCheckerTest, TestUndefinedValRefs) {
  d.parseString("val a: Int = b * c");
  scopeChecker.run(d.result);
  EXPECT_EQ(false, scopeChecker.isValid());
  ASSERT_EQ(2, scopeChecker.getUnknownIdentifiers().size());

  nth::IdentifierList &list = scopeChecker.getUnknownIdentifiers();
  EXPECT_EQ("b", list.front()->getValue()); list.pop_front();
  EXPECT_EQ("c", list.front()->getValue());
}

TEST_F(ScopeCheckerTest, TestMoreUndefinedValRefs) {
  d.parseString("val a: Int = b * 2\nval b: Int = 10");
  scopeChecker.run(d.result);
  EXPECT_EQ(false, scopeChecker.isValid());
  ASSERT_EQ(1, scopeChecker.getUnknownIdentifiers().size());

  nth::IdentifierList &list = scopeChecker.getUnknownIdentifiers();
  EXPECT_EQ("b", list.front()->getValue());
}

TEST_F(ScopeCheckerTest, TestPredefinedValRefs) {
  d.parseString("val b: Int = 10\nval a: Int = b * 2");
  scopeChecker.run(d.result);
  EXPECT_EQ(true, scopeChecker.isValid());
}

TEST_F(ScopeCheckerTest, TestOutOfOrderFunctionRef) {
  d.parseString("foo()\ndef foo(): Int { 10 }");
  scopeChecker.run(d.result);
  EXPECT_EQ(true, scopeChecker.isValid());
}

TEST_F(ScopeCheckerTest, TestValidArgUse) {
  d.parseString("def foo(a: Int): Int { a * 10 }");
  scopeChecker.run(d.result);
  EXPECT_EQ(true, scopeChecker.isValid());
}

TEST_F(ScopeCheckerTest, TestInvalidArgUse) {
  d.parseString("def foo(a: Int): Int { b * 10 }");
  scopeChecker.run(d.result);
  EXPECT_EQ(false, scopeChecker.isValid());
}

TEST_F(ScopeCheckerTest, TestNestedValDecl) {
  d.parseString("def foo(a: Int): Int { val b: Int = a * 10\nb }");
  scopeChecker.run(d.result);
  EXPECT_EQ(true, scopeChecker.isValid());
}

TEST_F(ScopeCheckerTest, TestOutOfOrderNestedValDecl) {
  d.parseString("def foo(a: Int): Int { val c: Int = b * 10\nval b: Int = 20 }");
  scopeChecker.run(d.result);
  EXPECT_EQ(false, scopeChecker.isValid());
}

TEST_F(ScopeCheckerTest, TestTypeAlias) {
  d.parseString("type Foo = Int\nval a: Foo = 10");
  scopeChecker.run(d.result);
  EXPECT_EQ(true, scopeChecker.isValid());
}

TEST_F(ScopeCheckerTest, TestTraitDef) {
  d.parseString("trait Foo\nFoo.new()");
  scopeChecker.run(d.result);
  // NOTE: scope checking cannot catch identifier references in method calls,
  // this is the responsibility of the type checker.
  // thus, identifiers used in field access (as is the case with Foo.new())
  // are unchecked during this pass.
  EXPECT_EQ(true, scopeChecker.isValid());
}

TEST_F(ScopeCheckerTest, TestTraitDefUseBeforeDecl) {
  d.parseString("Foo.new()\ntrait Foo");
  scopeChecker.run(d.result);
  EXPECT_EQ(true, scopeChecker.isValid());
}

TEST_F(ScopeCheckerTest, TestTraitSelfReference) {
  d.parseString("trait Foo { val f: Foo = 10 }");
  scopeChecker.run(d.result);
  EXPECT_EQ(true, scopeChecker.isValid());
}

TEST_F(ScopeCheckerTest, TestTraitCtorOutOfOrderValRef) {
  // This is valid because "a" is defined as both an instance
  // variable on the trait and a regular variable in the constructor
  // The former interpreation is given priority. However, "a" will not
  // have a value if this were executed. It is nonetheless valid.
  d.parseString("trait Foo { a\n val a: Int = 10 }");
  scopeChecker.run(d.result);
  EXPECT_EQ(true, scopeChecker.isValid());
}

TEST_F(ScopeCheckerTest, TestTraitCtorOutOfOrderMethodCall) {
  d.parseString("trait Foo { a()\n def a(): Int { 10 } }");
  scopeChecker.run(d.result);
  EXPECT_EQ(true, scopeChecker.isValid());
}

TEST(ScopePrimerTest, TestValDefinedinTraitScopeNotBlockScope) {
  FAIL();
//  d.parseString("trait Foo { val a: Foo = 10");
//  ScopePrimer primer;
//  d.result->accept(primer);
//  primer
}

TEST(ScopePrimerTest, TestDefDefinedInTraitScopeNotBlockScope) {
  FAIL();
}

TEST_F(ScopeCheckerTest, TestTraitAddsCtorSymbol) {
  d.parseString("trait Foo");
  scopeChecker.run(d.result);
  nth::SymbolTable *blockSymbolTable = d.result->getSymbolTable();
  ASSERT_NE(nullptr, blockSymbolTable);
  nth::Symbol *ctorSym = blockSymbolTable->findSymbol("foo_ctor");
  EXPECT_NE(nullptr, ctorSym);
}