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

typedef std::vector<Expression*> ExpressionList;

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
 protected:
  std::string value;
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
