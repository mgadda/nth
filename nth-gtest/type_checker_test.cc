//
//  type_checker_test.cc
//  nth
//
//  Created by Matt Gadda on 12/6/14.
//  Copyright (c) 2014 Matt Gadda. All rights reserved.
//
#include <gtest/gtest.h>
#include "type.h"
#include "ast.h"
#include "type_checker.h"
#include "symbol_table.h"

class TypeCheckerTest : public ::testing::Test {
 protected:
  nth::SymbolTable root;
  virtual void SetUp() {}
 private:
};

nth::Type& buildClass(std::string name, nth::SymbolTable &table, nth::Type *parent=nullptr) {
  nth::Identifier *ident = new nth::Identifier(name);
  nth::SimpleType *type = new nth::SimpleType(ident, nth::VariableSet(), nth::MethodSet(), parent);
  table.addSymbol(ident).setType(*type);
  return *type;
}

TEST_F(TypeCheckerTest, testBlock) {
  // TODO: setting up these fixtures is error prone.
  nth::Type &objectType = buildClass("Object", root);
  nth::Type &integerType = buildClass("Integer", root, &objectType);
  nth::FunctionType *plusMethodType = new nth::FunctionType({ &integerType }, &integerType);
  // does it make sense to create a FunctionType?
  nth::Symbol *plusSym = new nth::Symbol(new nth::Identifier("+"));
  plusSym->setType(*plusMethodType);
  integerType.getMethods().insert(plusSym);
  
  // create block with multiple expressions, each with different type
  // assert that type of block is set to type of last expression
  nth::Block *b = new nth::Block;
  b->setSymbolTable(root.beget());

  nth::Integer *i1 = new nth::Integer(1);
  nth::Integer *i2 = new nth::Integer(2);
  nth::Add *a = new nth::Add(nth::ExpressionPtr(i1), nth::ExpressionPtr(i2));
  b->insertAfter(a);
  nth::TypeChecker tc;
  // TODO: stop catching exceptions, once there's proper error handling in place
  EXPECT_NO_THROW(b->accept(tc));
  delete a;
}

TEST_F(TypeCheckerTest, testBlockWithExpressionsAndStatements) {
  // create block with multiple statements, each with different type
  // assert that type of block is set to type of last expression
}

TEST_F(TypeCheckerTest, testBlockWithOnlyStatements) {
  // assert return type is Object, useless but valid
  // it must be valid because some blocks might only contain class/function defs
}