/*
 * Types for representing Types during parsing
 */

// class Tuple represents the literal definition of a Tuple
// It could have been called TupleLiteral
// class TupleType represents a built-in type


#include "ast.h"

namespace nth {

class Type : public ASTNode {
 public:
  Identifier *getName() { return name; }

 protected:
  Type(Identifier *name) : name(name) {}
  Identifier *name;
};

class SimpleType : public Type {
 public:
  SimpleType(Identifier *name) : Type(name) {}

  void accept(Visitor &v) { v.visit(this); }

  Identifier *getName() { return name; }
};

class TemplatedType : public Type {
 public:
  TemplatedType(Identifier *name, TypeList &subtypes)
   : Type(name), subtypes(subtypes) {}

  TemplatedType(Identifier *name, TypeList &&subtypes)
   : Type(name), subtypes(subtypes) {}

  void accept(Visitor &v) { v.visit(this); }

  TypeList &getSubtypes() { return subtypes; }

 protected:
  TypeList subtypes;
};

class TupleType : public TemplatedType {
 public:
  TupleType(TypeList &subtypes)
   : TemplatedType(
       Identifier::forTemplatedType("Tuple", subtypes.size()),
       subtypes
     ) {}
};

class FunctionType : public TemplatedType {
 public:
  FunctionType(TypeList &argTypes, Type *returnType)
   : TemplatedType(
       Identifier::forTemplatedType("Function", argTypes.size() + 1),
       FunctionType::concat_ctr_args(argTypes, returnType)
     ) {}

  static TypeList concat_ctr_args(TypeList argList, Type *returnType);
};

}
