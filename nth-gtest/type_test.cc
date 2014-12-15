//
//  type_test.cc
//  nth
//
//  Created by Matt Gadda on 11/29/14.
//  Copyright (c) 2014 Matt Gadda. All rights reserved.
//

#include <vector>
#include <gtest/gtest.h>
#include "test_helper.h"
#include "type.h"

/*
 Object <- LUB(Foo, Bar)
 |- Foo
 |- Bar
 */
TEST(TypeTest, computeLUB_simple) {
  std::set<nth::Type*> types;
  nth::Type *root = new nth::SimpleType(new nth::Identifier("Object"), nth::VariableSet(), nth::MethodSet(), nullptr);

  types.insert(new nth::SimpleType(new nth::Identifier("Foo"), nth::VariableSet(), nth::MethodSet(), root));

  types.insert(new nth::SimpleType(new nth::Identifier("Bar"), nth::VariableSet(), nth::MethodSet(), root));

  ASSERT_EQ(root, nth::Type::computeLUB(types));
}

/*
 Object <- LUB(Baz, Foo)
 |- Foo
 |- Bar
    |-Baz
 */
TEST(TypeTest, computeLUB_offset) {
  std::set<nth::Type*> types;
  nth::Type *root = new nth::SimpleType(new nth::Identifier("Object"), nth::VariableSet(), nth::MethodSet(), nullptr);

  nth::SimpleType *fooType = new nth::SimpleType(new nth::Identifier("Foo"), nth::VariableSet(), nth::MethodSet(), root);
  nth::SimpleType *barType = new nth::SimpleType(new nth::Identifier("Bar"), nth::VariableSet(), nth::MethodSet(), root);
  nth::SimpleType *bazType = new nth::SimpleType(new nth::Identifier("Baz"), nth::VariableSet(), nth::MethodSet(), barType);

  types.insert(bazType);
  types.insert(fooType);

  ASSERT_EQ(root, nth::Type::computeLUB(types));
}

/*
 Object <- LUB(Qux, Norf, Foo)
 |- Foo
 |- Bar
    |-Baz
      |- Norf
    |-Qux
 */
TEST(TypeTest, computeLUB_multi) {
  std::set<nth::Type*> types;
  nth::Type *root = new nth::SimpleType(new nth::Identifier("Object"), nth::VariableSet(), nth::MethodSet(), nullptr);

  nth::SimpleType *fooType = new nth::SimpleType(new nth::Identifier("Foo"), nth::VariableSet(), nth::MethodSet(), root);
  nth::SimpleType *barType = new nth::SimpleType(new nth::Identifier("Bar"), nth::VariableSet(), nth::MethodSet(), root);
  nth::SimpleType *bazType = new nth::SimpleType(new nth::Identifier("Baz"), nth::VariableSet(), nth::MethodSet(), barType);
  nth::SimpleType *quxType = new nth::SimpleType(new nth::Identifier("Qux"), nth::VariableSet(), nth::MethodSet(), barType);
  nth::SimpleType *norfType = new nth::SimpleType(new nth::Identifier("Norf"), nth::VariableSet(), nth::MethodSet(), bazType);

  types.insert(quxType);
  types.insert(norfType);
  types.insert(fooType);

  ASSERT_EQ(root, nth::Type::computeLUB(types));
}

/*
 Object <- LUB(Object, Foo, Foo)
 |- Foo
 */
TEST(TypeTest, computeLUB_dup) {
  std::set<nth::Type*> types;
  nth::Type *root = new nth::SimpleType(new nth::Identifier("Object"), nth::VariableSet(), nth::MethodSet(), nullptr);

  nth::SimpleType *fooType = new nth::SimpleType(new nth::Identifier("Foo"), nth::VariableSet(), nth::MethodSet(), root);

  types.insert(fooType);
  types.insert(root);
  types.insert(fooType);

  ASSERT_EQ(root, nth::Type::computeLUB(types));
}

TEST(TypeTest, computeLUB_same) {
  std::set<nth::Type*> types;
  nth::Type *root = new nth::SimpleType(new nth::Identifier("Object"), nth::VariableSet(), nth::MethodSet(), nullptr);

  nth::SimpleType *fooType = new nth::SimpleType(new nth::Identifier("Foo"), nth::VariableSet(), nth::MethodSet(), root);

  types.insert(fooType);
  types.insert(fooType);

  ASSERT_EQ(fooType, nth::Type::computeLUB(types));
}