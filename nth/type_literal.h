//
//  Types for representing types during parsing (these are ast nodes)
//  type_literal.h
//  nth
//
//  Created by Matt Gadda on 12/14/14.
//  Copyright (c) 2014 Matt Gadda. All rights reserved.
//

#ifndef __nth__type_literal__
#define __nth__type_literal__

#include "ast.h"

namespace nth {

// Represents a type reference or definition in the AST
class TypeLiteral : public ASTNode {
public:
  TypeLiteral(Identifier *name) : name(name) {}

  Identifier *getName() { return name; }

  virtual void accept(Visitor &v)=0;

protected:
  Identifier *name;
};

// These classes may not actually need to inherit Type
// For the purposes of representing /what/ was parsed
// everything is either a definition or a reference
// and there may (or may not) be an occasion where we
// care to differentiate.
class TypeRef : public TypeLiteral {
public:
  virtual void accept(Visitor &v)=0;

protected:
  TypeRef(Identifier *name) : TypeLiteral(name) {}
};

class TypeDef : public TypeLiteral {
protected:
  TypeDef(Identifier *name) : TypeLiteral(name) {}
};

class SimpleTypeRef : public TypeRef {
public:
  SimpleTypeRef(Identifier *name) : TypeRef(name) {}

  void accept(Visitor &v) { v.visit(this); }

  Identifier *getName() { return name; }
};

class SimpleTypeDef : public TypeDef {
public:
  SimpleTypeDef(Identifier *name) : TypeDef(name) {}

  void accept(Visitor &v) { v.visit(this); }

  Identifier *getName() { return name; }
};

class TemplatedTypeRef : public TypeRef {
public:
  TemplatedTypeRef(Identifier *name, TypeRefList &subtypes)
  : TypeRef(name), subtypes(subtypes) {}

  TemplatedTypeRef(Identifier *name, TypeRefList &&subtypes)
  : TypeRef(name), subtypes(subtypes) {}

  void accept(Visitor &v) { v.visit(this); }

  TypeRefList &getSubtypes() { return subtypes; }

protected:
  TypeRefList subtypes;
};

// TODO: this may not really be in use
// TODO: replace with nodes like trait, class, type alias def
// For defining templated classes: class Foo[A, B]
// Foo, A, and B are all being defined here
class TemplatedTypeDef : public TypeDef {
public:
  TemplatedTypeDef(Identifier *name, TypeDefList &subtypes)
  : TypeDef(name), subtypes(subtypes) {}

  TemplatedTypeDef(Identifier *name, TypeDefList &&subtypes)
  : TypeDef(name), subtypes(subtypes) {}

  void accept(Visitor &v) { v.visit(this); }

  TypeDefList &getSubtypes() { return subtypes; }

protected:
  TypeDefList subtypes;
};

// class Tuple represents the literal definition of a Tuple
// It could have been called TupleLiteral
// class TupleType represents a built-in type

class TupleTypeRef : public TemplatedTypeRef {
public:
  TupleTypeRef(TypeRefList &subtypes)
  : TemplatedTypeRef(
                     Identifier::forTemplatedType("Tuple", subtypes.size()),
                     subtypes
                     ) {}
};

class FunctionTypeRef : public TemplatedTypeRef {
public:
  FunctionTypeRef(TypeRefList &argTypes, TypeRef *returnType)
  : TemplatedTypeRef(
                     Identifier::forTemplatedType("Function", argTypes.size()),
                     FunctionTypeRef::concat_ctr_args(argTypes, returnType)
                     ) {}

  static TypeRefList concat_ctr_args(TypeRefList argList, TypeRef *returnType);
};

}
#endif /* defined(__nth__type_literal__) */
