//
//  ast.cpp
//  nth
//
//  Created by Matt Gadda on 6/7/14.
//  Copyright (c) 2014 Matt Gadda. All rights reserved.
//

#include "ast.h"

using namespace nth;

Block::Block() {}

Block::Block(Expression *expr) {
  expressions.push_back(expr);
}

void Block::insertAfter(Expression *expr) {
  expressions.push_back(expr);
}

ExpressionList &Block::getExpressions() {
  return expressions;
}

void Add::accept(Visitor &v) {
  v.visit(this);
  BinaryOperation::accept(v);
}

void Subtract::accept(Visitor &v) {
  v.visit(this);
  BinaryOperation::accept(v);
}

void Multiply::accept(Visitor &v) {
  v.visit(this);
  BinaryOperation::accept(v);
}

void Divide::accept(Visitor &v) {
  v.visit(this);
  BinaryOperation::accept(v);
}

void Exponentiate::accept(Visitor &v) {
  v.visit(this);
  BinaryOperation::accept(v);
}

void Modulo::accept(Visitor &v) {
  v.visit(this);
  BinaryOperation::accept(v);
}

void BitShiftLeft::accept(Visitor &v) {
  v.visit(this);
  BinaryOperation::accept(v);
}

void BitShiftRight::accept(Visitor &v) {
  v.visit(this);
  BinaryOperation::accept(v);
}

void BitwiseOr::accept(Visitor &v) {
  v.visit(this);
  BinaryOperation::accept(v);
}

void BitwiseAnd::accept(Visitor &v) {
  v.visit(this);
  BinaryOperation::accept(v);
}

void BitwiseNot::accept(Visitor &v) {
  UnaryOperation::accept(v);
  v.visit(this);
}

void LogicalOr::accept(Visitor &v) {
  v.visit(this);
  BinaryOperation::accept(v);
}

void LogicalAnd::accept(Visitor &v) {
  v.visit(this);
  BinaryOperation::accept(v);
}

void LogicalNot::accept(Visitor &v) {
  UnaryOperation::accept(v);
  v.visit(this);
}

void Comparison::accept(Visitor &v) {
  v.visit(this);
  BinaryOperation::accept(v);
}
