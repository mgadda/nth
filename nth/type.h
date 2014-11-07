/*
 * Types for representing Types during parsing
 */


#include "ast.h"

namespace nth {

class Type : public ASTNode {
 public:
  Identifier *getName() { return name; }

  virtual void accept(Visitor &v)=0;

 protected:
  Type(Identifier *name) : name(name) {}
  Identifier *name;
};

// These classes may not actually need to inherit Type
// For the purposes of representing /what/ was parsed
// everything is either a definition or a reference
// and there may (or may not) be an occasion where we 
// care to differentiate.
class TypeRef : public Type {
 public:
  virtual void accept(Visitor &v)=0;
  
 protected:
  TypeRef(Identifier *name) : Type(name) {}
};

class TypeDef : public Type {
 protected:
  TypeDef(Identifier *name) : Type(name) {}
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

class TemplatedTypeDef : public TypeDef {
 public:
  TemplatedTypeDef(Identifier *name, TypeList &subtypes)
   : TypeDef(name), subtypes(subtypes) {}

  TemplatedTypeDef(Identifier *name, TypeList &&subtypes)
   : TypeDef(name), subtypes(subtypes) {}

  void accept(Visitor &v) { v.visit(this); }

  TypeList &getSubtypes() { return subtypes; }

 protected:
  TypeList subtypes;
};

// class Tuple represents the literal definition of a Tuple
// It could have been called TupleLiteral
// class TupleType represents a built-in type

class TupleType : public TemplatedTypeRef {
 public:
  TupleType(TypeRefList &subtypes)
   : TemplatedTypeRef(
       Identifier::forTemplatedType("Tuple", subtypes.size()),
       subtypes
     ) {}
};

class FunctionType : public TemplatedTypeRef {
 public:
  FunctionType(TypeRefList &argTypes, TypeRef *returnType)
   : TemplatedTypeRef(
       Identifier::forTemplatedType("Function", argTypes.size()),
       FunctionType::concat_ctr_args(argTypes, returnType)
     ) {}

  static TypeRefList concat_ctr_args(TypeRefList argList, TypeRef *returnType);
};

}
