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

#include "ast_visitor.h"

namespace nth {

class Type;

// Anything at all
class ASTNode : public Visitable {
 public:
  virtual ~ASTNode() {}
};

// Anything that has a value
class Expression : public ASTNode {
 public:
  virtual ~Expression() {}
};

typedef std::vector<ASTNode*> NodeList;
typedef std::vector<Expression*> ExpressionList;
typedef std::pair<nth::String*, Expression*> KeyValuePair;
typedef std::vector<KeyValuePair> ExpressionMap;
typedef std::unique_ptr<Expression> ExpressionPtr;
typedef std::list<Argument*> ArgList;
typedef std::list<Type*> TypeList;

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
  Integer(long value): value(value) {}
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

  static Identifier *forTemplatedType(std::string name, int subtypeCount);

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
  Array(ExpressionList &exprlist) : values(exprlist) {}
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
  Map(ExpressionMap &exprmap) : values(exprmap) {}
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
  UnaryOperation(ExpressionPtr value) : value(std::move(value)) {}
  ExpressionPtr &getValue() { return value; }

  void accept(Visitor &v) { v.visit(this); }

 protected:
  ExpressionPtr value;
};

class BinaryOperation : public Expression {
 public:
  BinaryOperation(ExpressionPtr left,
                  ExpressionPtr right)
    : left(std::move(left)), right(std::move(right)) {}

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

  Range(Integer *start, Integer *end, Exclusivity exclusivity) :
    start(start),
    end(end),
    exclusivity(exclusivity) {}

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
  Tuple() {}
  Tuple(ExpressionList &values) : values(values) {}
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

class TupleFieldAccess : public BinaryOperation {
 public:
  TupleFieldAccess(Expression *tupleExpr, Integer *fieldIndex)
  : BinaryOperation(ExpressionPtr(tupleExpr), ExpressionPtr(fieldIndex)) {}

  void accept(Visitor &v);
};


class Argument : public ASTNode {
 public:
  Argument(Identifier *name, Type *type) : name(name), type(type) {}

  void accept(Visitor &v) { v.visit(this); }

  Identifier *getName() { return name; }
  Type *getType() { return type; }

 protected:
  Identifier *name;
  Type *type;
};

class FunctionDef : public ASTNode {
 public:
  FunctionDef(Identifier *name, ArgList &argList, Type *returnType, Block *block)
    : name(name), argList(argList), returnType(returnType), block(block) {}

  void accept(Visitor &v) { v.visit(this); }

  Identifier *getName() { return name; }
  ArgList &getArguments() { return argList; }
  Type *getReturnType() { return returnType; }
  Block *getBlock() { return block; }

 protected:
  Identifier      *name;
  ArgList         argList;
  Type            *returnType;
  Block           *block;
};

}
#endif /* defined(__nth__ast__) */
