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
  BinaryOperation::accept(v);
  v.visit(this);
}

void Subtract::accept(Visitor &v) {
  BinaryOperation::accept(v);
  v.visit(this);
}

void Multiply::accept(Visitor &v) {
  BinaryOperation::accept(v);
  v.visit(this);
}

void Divide::accept(Visitor &v) {
  BinaryOperation::accept(v);
  v.visit(this);
}

void Exponentiate::accept(Visitor &v) {
  BinaryOperation::accept(v);
  v.visit(this);
}

void Modulo::accept(Visitor &v) {
  BinaryOperation::accept(v);
  v.visit(this);
}

void BitShiftLeft::accept(Visitor &v) {
  BinaryOperation::accept(v);
  v.visit(this);
}

void BitShiftRight::accept(Visitor &v) {
  BinaryOperation::accept(v);
  v.visit(this);
}

void BitwiseOr::accept(Visitor &v) {
  BinaryOperation::accept(v);
  v.visit(this);
}

void BitwiseAnd::accept(Visitor &v) {
  BinaryOperation::accept(v);
  v.visit(this);
}

void LogicalOr::accept(Visitor &v) {
  BinaryOperation::accept(v);
  v.visit(this);
}

void LogicalAnd::accept(Visitor &v) {
  BinaryOperation::accept(v);
  v.visit(this);
}

void LogicalNot::accept(Visitor &v) {
  UnaryOperation::accept(v);
  v.visit(this);
}

void Comparison::accept(Visitor &v) {
  BinaryOperation::accept(v);
  v.visit(this);
}