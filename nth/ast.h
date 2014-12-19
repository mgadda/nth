//
//  ast.h
//  nth
//
//  Created by Matt Gadda on 6/7/14.
//  Copyright (c) 2014 Matt Gadda. All rights reserved.
//

#ifndef __nth__ast__
#define __nth__ast__

#include <iostream>
#include <vector>
#include <list>
#include <memory>

#include <iostream>

#include "location.hh"
#include "ast_visitor.h"

namespace nth {

class Type;
class TypeDef;
class TypeRef;
class SymbolTable;

// Anything at all, really
class ASTNode : public Visitable {
 public:
  ASTNode() : _symbolTable(nullptr), _parent(nullptr) {}
  ASTNode(yy::location &l) : loc(l), _symbolTable(nullptr), _parent(nullptr) {}
  virtual ~ASTNode() {}

  yy::location &getLocation() { return loc; }

  void setSymbolTable(SymbolTable *symbolTable) { _symbolTable = symbolTable; }
  SymbolTable *getSymbolTable() { return _symbolTable; }

  void setParent(ASTNode *node);
  ASTNode *getParent() { return _parent; }

  SymbolTable *getNearestSymbolTable();

 protected:
  yy::location loc; // where we encounter this node while parsing
  SymbolTable *_symbolTable; // used during scope checking to retain info between passes
  ASTNode *_parent;
};

class DummyNode : public nth::ASTNode {
  virtual ~DummyNode() {}
  void accept(nth::Visitor &v) { v.visit(this); }
};


// Anything that has a value
class Expression : public ASTNode {
 public:
  Expression() {}
  Expression(yy::location &l) : ASTNode(l) {}
  virtual ~Expression() {}

  void setType(Type *type) { _type = type; }
  Type *getType() { return _type; }

 protected:
  Type *_type;
};

typedef std::vector<ASTNode*> NodeList;
typedef std::vector<Expression*> ExpressionList;
typedef std::pair<nth::String*, Expression*> KeyValuePair;
typedef std::vector<KeyValuePair> ExpressionMap;
typedef std::unique_ptr<Expression> ExpressionPtr;
typedef std::list<Argument*> ArgList;
typedef std::list<Type*> TypeList;
typedef std::list<TypeRef*> TypeRefList;
typedef std::list<TypeDef*> TypeDefList;

class Block : public Expression {
 public:
  Block();
  Block(ASTNode *node);
  void insertAfter(ASTNode *node);
  NodeList &getNodes();

  // Visitable
  void accept(Visitor &v) { v.visit(this); }

 protected:
  NodeList nodes;
};

class Integer : public Expression {
 public:
  Integer(long value): Expression(), value(value) {}
  Integer(Integer &&other) : value(other.value) {}
  virtual ~Integer() {}

  // Visitable
  void accept(Visitor &v) { v.visit(this); }

  bool operator==(const int &i) const { return value == i; }
  bool operator==(const long &i) const { return value == i; }
  operator long() const { return value; }

  long getValue() { return value; }
 protected:
  long value;
};

class Float : public Expression {
 public:
  Float(double value): value(value) {}
  Float(Float &&other) : value(other.value) {}
  virtual ~Float() {}

  // Visitable
  void accept(Visitor &v) { v.visit(this); }

  bool operator==(const float &i) const { return value == i; }
  bool operator==(const double &i) const { return value == i; }
  operator double() const { return value; }

  double getValue() { return value; }
protected:
  double value;
};

class String : public Expression {
 public:
  String(std::string value): value(value) {}
  String(String &&other) : value(std::move(other.value)) {}

  // Visitable
  void accept(Visitor &v) { v.visit(this); }

  bool operator==(const std::string &i) const { return value == i; }
  bool operator==(const char *i) const { return value == i; }
  operator const std::string() const { return value; }
  operator const char*() const { return value.c_str(); }

  std::string getValue() { return value; }
 protected:
  std::string value;
};

class Boolean : public Expression {
 public:
  Boolean(bool value): value(value) {}
  Boolean(Boolean &&other) : value(other.value) {}

  bool operator==(const bool &i) const { return value == i; }
  operator const bool() const { return value; }

 protected:
  bool value;
};

class True : public Boolean {
 public:
  True() : Boolean(true) {}

  // Visitable
  void accept(Visitor &v) { v.visit(this); }
};

class False : public Boolean {
 public:
  False() : Boolean(false) {}

  // Visitable
  void accept(Visitor &v) { v.visit(this); }
};

class Identifier : public Expression {
 public:
  Identifier(std::string value) : value(value) {}
  Identifier(Identifier &&other) : value(other.value) {}
  Identifier(std::string value, yy::location &loc)
   : Expression(loc), value(value) {}

  static Identifier *forTemplatedType(std::string name, size_t subtypeCount);

  // Visitable
  void accept(Visitor &v) { v.visit(this); }

  bool operator==(const std::string s) const { return value == s; }
  bool operator==(const Identifier &i) const { return value == i.value; }
  bool operator==(const char *c) const { return value == c; }
  operator const char*() const { return value.c_str(); }
  
  std::string getValue() { return value; }
 protected:
  std::string value;

};

class Array : public Expression {
 public:
  Array() {}
  Array(ExpressionList &exprlist);
  Array(Array &&other) : values(other.values) {}

  // Visitable
  void accept(Visitor &v) { v.visit(this); }

  bool operator==(const Array &i) const { return values == i.values; }
  const ExpressionList &getValues() { return values; }
 protected:
  ExpressionList values;
};

class Map : public Expression {
 public:
  Map() {}
  Map(ExpressionMap &exprmap);
  Map(Map &&other) : values(other.values) {}

  // Visitable
  void accept(Visitor &v) { v.visit(this); }

  bool operator==(const Map &m) const { return values == m.values; }
  const ExpressionMap &getValues() { return values; }
  //const Expression *getValue(ComparableExpr &key);
 protected:
  ExpressionMap values;
};

class UnaryOperation : public Expression {
 public:
  UnaryOperation(ExpressionPtr value);
  ExpressionPtr &getValue() { return value; }

  void accept(Visitor &v) { v.visit(this); }

 protected:
  ExpressionPtr value;
};

class BinaryOperation : public Expression {
 public:
  BinaryOperation(ExpressionPtr left, ExpressionPtr right);

  ExpressionPtr &getLeftValue() { return left; }
  ExpressionPtr &getRightValue() { return right; }

  void accept(Visitor &v) { v.visit(this); }

 protected:
  ExpressionPtr left;
  ExpressionPtr right;
};

class Add : public BinaryOperation {
 public:
  Add(ExpressionPtr left,
      ExpressionPtr right)
    : BinaryOperation(std::move(left), std::move(right)) {}

  // Visitable
  void accept(Visitor &v);
};

class Subtract : public BinaryOperation {
public:
  Subtract(ExpressionPtr left,
           ExpressionPtr right)
    : BinaryOperation(std::move(left), std::move(right)) {}

  // Visitable
  void accept(Visitor &v);
};

class Multiply : public BinaryOperation {
public:
  Multiply(ExpressionPtr left,
           ExpressionPtr right)
    : BinaryOperation(std::move(left), std::move(right)) {}

  // Visitable
  void accept(Visitor &v);
};

class Divide : public BinaryOperation {
public:
  Divide(ExpressionPtr left,
         ExpressionPtr right)
    : BinaryOperation(std::move(left), std::move(right)) {}

  // Visitable
  void accept(Visitor &v);
};

class Exponentiate : public BinaryOperation {
public:
  Exponentiate(ExpressionPtr left,
         ExpressionPtr right)
    : BinaryOperation(std::move(left), std::move(right)) {}

  // Visitable
  void accept(Visitor &v);
};

class Modulo : public BinaryOperation {
public:
  Modulo(ExpressionPtr left,
         ExpressionPtr right)
    : BinaryOperation(std::move(left), std::move(right)) {}

  // Visitable
  void accept(Visitor &v);
};

class BitShiftLeft : public BinaryOperation {
public:
  BitShiftLeft(ExpressionPtr left,
         std::unique_ptr<Integer> right)
    : BinaryOperation(std::move(left), std::move(right)) {}

  // Visitable
  void accept(Visitor &v);
};

class BitShiftRight : public BinaryOperation {
public:
  BitShiftRight(ExpressionPtr left,
         std::unique_ptr<Integer> right)
    : BinaryOperation(std::move(left), std::move(right)) {}

  // Visitable
  void accept(Visitor &v);
};

class BitwiseOr : public BinaryOperation {
public:
  BitwiseOr(ExpressionPtr left,
         ExpressionPtr right)
    : BinaryOperation(std::move(left), std::move(right)) {}

  // Visitable
  void accept(Visitor &v);
};

class BitwiseAnd : public BinaryOperation {
public:
  BitwiseAnd(ExpressionPtr left,
         ExpressionPtr right)
    : BinaryOperation(std::move(left), std::move(right)) {}

  // Visitable
  void accept(Visitor &v);
};

class BitwiseNot : public UnaryOperation {
public:
  BitwiseNot(ExpressionPtr expr)
  : UnaryOperation(std::move(expr)) {}

  // Visitable
  void accept(Visitor &v);
};

class LogicalOr : public BinaryOperation {
public:
  LogicalOr(ExpressionPtr left,
            ExpressionPtr right)
  : BinaryOperation(std::move(left), std::move(right)) {}

  // Visitable
  void accept(Visitor &v);
};

class LogicalAnd : public BinaryOperation {
public:
  LogicalAnd(ExpressionPtr left,
             ExpressionPtr right)
  : BinaryOperation(std::move(left), std::move(right)) {}

  // Visitable
  void accept(Visitor &v);
};

class LogicalNot : public UnaryOperation {
public:
  LogicalNot(ExpressionPtr expr)
  : UnaryOperation(std::move(expr)) {}

  // Visitable
  void accept(Visitor &v);
};

class Range : public Expression {
 public:
  enum class Exclusivity { Exclusive, Inclusive };

  Range(Integer *start, Integer *end, Exclusivity exclusivity);

  void accept(Visitor &v) { v.visit(this); }

  Integer *getStart() { return start; }
  Integer *getEnd() { return end; }
  Exclusivity getExclusivity() { return exclusivity; }
 protected:
  Integer *start;
  Integer *end;
  Exclusivity exclusivity;
};

class Tuple : public Expression {
 public:
  Tuple(ExpressionList &values);
  ExpressionList &getExpressions();

  // Visitable
  void accept(Visitor &v) { v.visit(this); }

  const ExpressionList &getValues() { return values; }
 protected:
  ExpressionList values;
};

class Comparison : public BinaryOperation {
 public:
  enum class Type {
    Equality,
    Inequality,
    LessThan,
    GreaterThan,
    LessThanOrEqualTo,
    GreaterThanOrEqualTo
  };

  Comparison(ExpressionPtr left, ExpressionPtr right, Type type)
    : BinaryOperation(std::move(left), std::move(right)), type(type) {}

  Type getType() { return type; }

  // Visitable
  void accept(Visitor &v);

 protected:
  Type type;
};

class Subscript : public BinaryOperation {
 public:
  // TODO: type checker should verify that key is a string
  // and that expr is a thing-that-supports-subscript-operations
  Subscript(Expression *expr, Expression *key)
  : BinaryOperation(ExpressionPtr(expr), ExpressionPtr(key)) {}

  void accept(Visitor &v);
};

class FieldAccess : public BinaryOperation {
 public:
  FieldAccess(Expression *object, Identifier *field)
  : BinaryOperation(ExpressionPtr(object), ExpressionPtr(field)) {}

  Identifier &getField() {
    return dynamic_cast<Identifier&>(*BinaryOperation::getRightValue()); }

  void accept(Visitor &v);
};

class TupleFieldAccess : public BinaryOperation {
public:
  TupleFieldAccess(Expression *object, Integer *index)
  : BinaryOperation(ExpressionPtr(object), ExpressionPtr(index)) {}

  Integer &getIndex() { return dynamic_cast<Integer&>(*BinaryOperation::getRightValue()); }
  void accept(Visitor &v);
};


class Argument : public ASTNode {
 public:
  Argument(Identifier *name, TypeRef *type);

  void accept(Visitor &v) { v.visit(this); }

  Identifier *getName() { return name; }
  TypeRef *getType() { return type; }

 protected:
  Identifier *name;
  TypeRef *type;
};

// def makeTea(): Tea { ... }
class FunctionDef : public ASTNode {
 public:
  FunctionDef(Identifier *name, ArgList &argList, TypeRef *returnType, Block *block, TypeDefList &typeParameters);

  void accept(Visitor &v) { v.visit(this); }

  Identifier *getName() { return name; }
  TypeDefList &getTypeParameters() { return typeParameters; }
  ArgList &getArguments() { return argList; }
  TypeRef *getReturnType() { return returnType; }
  Block *getBlock() { return block; }

 protected:
  Identifier      *name;
  ArgList         argList;
  TypeRef         *returnType;
  Block           *block;
  TypeDefList     typeParameters;
};

// { (x: Int, y: Int): Int => x + y } 
class LambdaDef : public Expression {
 public:
  LambdaDef(ArgList &argList, TypeRef *returnType, Expression *body);

  void accept(Visitor &v) { v.visit(this); }

  ArgList &getArguments() { return argList; }
  TypeRef *getReturnType() { return returnType; }
  Expression *getBody() { return body; }

 protected:
  ArgList         argList;
  TypeRef         *returnType;
  Expression      *body;
};

// makeTea()
class FunctionCall : public Expression {
 public:
  FunctionCall(Expression *callable, ExpressionList &arguments);

  void accept(Visitor &v) { v.visit(this); }

  Expression *getCallable() { return callable; }
  ExpressionList &getArguments() { return arguments; }
 protected:
  Expression *callable;
  ExpressionList arguments;
};

// val a: Int = 10
class VariableDef : public ASTNode {
 public:
  VariableDef(Identifier *name, TypeRef *varType, Expression *value);

  void accept(Visitor &v) { v.visit(this); }

  Identifier *getName() { return name; }
  TypeRef *getVarType() { return varType; }
  Expression *getValue() { return value; }

 protected:
  Identifier *name;
  TypeRef *varType;
  Expression *value;
};

// if (expr) { something } else { something_else }
class IfElse : public Expression {
 public:
  IfElse(Expression *condExpr, Block *ifBlock, Block *elseBlock);

  void accept(Visitor &v) { v.visit(this); }

  Expression *getCond() { return condExpr; }
  Block *getIfBlock() { return ifBlock; }
  Block *getElseBlock() { return elseBlock; }

 protected:
  Expression *condExpr;
  Block *ifBlock;
  Block *elseBlock;
};

// type IntList = Array[Int]
class TypeAliasDef : public ASTNode {
 public:
  TypeAliasDef(TypeDef *lType, TypeRef *rType);

  void accept(Visitor &v) { v.visit(this); }

  TypeDef *getLType() { return lType; }
  TypeRef *getRType() { return rType; }

 protected:
  TypeDef *lType;
  TypeRef *rType;
};

class TraitDef : public ASTNode {
 public:
  TraitDef(
    Identifier *name,
    TypeDefList *typeParameters,
    ArgList *constructorArgs,
    Block *block);

  void accept(Visitor &v) { v.visit(this); }

  Identifier *getName() { return name; }
  TypeDefList *getTypeParameters() { return typeParameters; }
  ArgList *getCtorArgs() { return ctorArgs; }
  Block *getBlock() { return block; }
 protected:
  Identifier *name;
  TypeDefList *typeParameters;
  ArgList     *ctorArgs;
  Block       *block;
};

}
#endif /* defined(__nth__ast__) */
