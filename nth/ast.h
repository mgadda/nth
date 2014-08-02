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
#include <memory>

namespace nth {

enum Type {
  IntegerType,
  StringType,
  AddType,
  SubtractType,
  MultiplyType,
  DivideType,
  BooleanNegationType
};

class Expression {
 public:
  virtual ~Expression() {}
};

class String;

typedef std::vector<Expression*> ExpressionList;
typedef std::pair<nth::String*, Expression*> KeyValuePair;
typedef std::vector<KeyValuePair> ExpressionMap;

class Block : Expression {
 public:
  Block();
  Block(Expression *expr);
  void insertAfter(Expression *expr);
  ExpressionList &getExpressions();
 protected:
  ExpressionList expressions;
};

class Integer : public Expression {
 public:
  Integer(long value): value(value) {}
  Integer(Integer &&other) : value(other.value) {}
  virtual ~Integer() {}

  bool operator==(const int &i) const { return value == i; }
  bool operator==(const long &i) const { return value == i; }
  operator long() const { return value; }
 protected:
  long value;
};

class Float : public Expression {
 public:
  Float(double value): value(value) {}
  Float(Float &&other) : value(other.value) {}
  virtual ~Float() {}

  bool operator==(const float &i) const { return value == i; }
  bool operator==(const double &i) const { return value == i; }
  operator double() const { return value; }
protected:
  double value;
};

class String : public Expression {
 public:
  String(std::string value): value(value) {}
  String(String &&other) : value(std::move(other.value)) {}

  bool operator==(const std::string &i) const { return value == i; }
  bool operator==(const char *i) const { return value == i; }
  operator const std::string() const { return value; }
  operator const char*() const { return value.c_str(); }
 
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
};

class False : public Boolean {
 public:
  False() : Boolean(false) {}
};

class Identifier : public Expression {
 public:
  Identifier(std::string value) : value(value) {}
  Identifier(Identifier &&other) : value(other.value) {}

  bool operator==(const std::string s) const { return value == s; }
  bool operator==(const Identifier &i) const { return value == i.value; }
  bool operator==(const char *c) const { return value == c; }
  operator const char*() const { return value.c_str(); }
 protected:
  std::string value;
};

class Array : public Expression {
 public:
  Array() {}
  Array(ExpressionList &exprlist) : values(exprlist) {}
  Array(Array &&other) : values(other.values) {}

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
  bool operator==(const Map &m) const { return values == m.values; }
  const ExpressionMap &getValues() { return values; }
  //const Expression *getValue(ComparableExpr &key);
 protected:
  ExpressionMap values;
};

class UnaryOperation : public Expression {
 public:
  UnaryOperation(std::unique_ptr<Expression> value) : value(std::move(value)) {}
 protected:
  std::unique_ptr<Expression> value;
};

class BooleanNegation : public UnaryOperation {
 public:
  BooleanNegation(std::unique_ptr<Expression> value): UnaryOperation(std::move(value)) {}
};

class BinaryOperation : public Expression {
 public:
  BinaryOperation(std::unique_ptr<Expression> left,
                  std::unique_ptr<Expression> right)
    : left(std::move(left)), right(std::move(right)) {}

 protected:
  std::unique_ptr<Expression> left;
  std::unique_ptr<Expression> right;
};

class Add : public BinaryOperation {
 public:
  Add(std::unique_ptr<Expression> left,
      std::unique_ptr<Expression> right)
    : BinaryOperation(std::move(left), std::move(right)) {}
};

class Subtract : public BinaryOperation {
public:
  Subtract(std::unique_ptr<Expression> left,
           std::unique_ptr<Expression> right)
    : BinaryOperation(std::move(left), std::move(right)) {}
};

class Multiply : public BinaryOperation {
public:
  Multiply(std::unique_ptr<Expression> left,
           std::unique_ptr<Expression> right)
    : BinaryOperation(std::move(left), std::move(right)) {}
};

class Divide : public BinaryOperation {
public:
  Divide(std::unique_ptr<Expression> left,
         std::unique_ptr<Expression> right)
    : BinaryOperation(std::move(left), std::move(right)) {}
};

}
#endif /* defined(__nth__ast__) */
