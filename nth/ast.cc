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

Block::Block(Expression expr) {
  expressions.push_back(expr);
}

void Block::insertAfter(Expression expr) {
  expressions.push_back(expr);
}

ExpressionList &Block::getExpressions() {
  return expressions;
}

