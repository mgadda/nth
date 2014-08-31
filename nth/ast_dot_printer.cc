//
//  ast_dot_printer.cc
//  nth
//
//  Created by Matt Gadda on 8/14/14.
//  Copyright (c) 2014 Matt Gadda. All rights reserved.
//

#include "ast_dot_printer.h"

AstDotPrinter::AstDotPrinter() {
}

std::string AstDotPrinter::getOutput() {
  ast_output << "digraph ast {\n";

  for (auto node : nodes) {

    ast_output << (int64_t)node.first
               << " [label=\"" << node.second << "\"];\n";
  }
  for (auto edge : edges) {
    ast_output << (int64_t)edge.first
               << " -> "
               << (int64_t)edge.second
               << ";\n";
  }
  ast_output << "}";
  return ast_output.str();
}

void AstDotPrinter::visit(nth::Block *block) {
  nodes[block] = "block";

  for (auto child : block->getNodes()) {
    if (child) {
      edges.push_back(std::make_pair(block, child));
      child->accept(*this);
    }
  }
}

void AstDotPrinter::visit(nth::String *string) {
  nodes[string] = string->getValue();
}

void AstDotPrinter::visit(nth::Integer *integer) {
  nodes[integer] = std::to_string(integer->getValue());
}

void AstDotPrinter::visit(nth::Float *flt) {
  nodes[flt] = std::to_string(flt->getValue());
}

void AstDotPrinter::visit(nth::True *tru) {
  nodes[tru] = "true";
}

void AstDotPrinter::visit(nth::False *flse) {
  nodes[flse] = "false";
}

void AstDotPrinter::visit(nth::Identifier *ident) {
  nodes[ident] = ident->getValue();
}

void AstDotPrinter::visit(nth::Array *array) {
  nodes[array] = "array";

  for (auto child : array->getValues()) {
    if (child) {
      edges.push_back(std::make_pair(array, child));
      child->accept(*this);
    }
  }
}

void AstDotPrinter::visit(nth::Map *map) {
  nodes[map] = "map";

  for (auto child : map->getValues()) {
    edges.push_back(std::make_pair(map, child.first));
    edges.push_back(std::make_pair(child.first, child.second));
    child.first->accept(*this);
    child.second->accept(*this);
  }
}

void AstDotPrinter::visit(nth::BinaryOperation *bin_op) {
  nth::ExpressionPtr &left = bin_op->getLeftValue();
  nth::ExpressionPtr &right = bin_op->getRightValue();

  edges.push_back(std::make_pair(bin_op, right.get()));
  edges.push_back(std::make_pair(bin_op, left.get()));

  left->accept(*this);
  right->accept(*this);
}

void AstDotPrinter::visit(nth::UnaryOperation *un_op) {
  nth::ExpressionPtr &value = un_op->getValue();

  edges.push_back(std::make_pair(un_op, value.get()));

  value->accept(*this);
}

void AstDotPrinter::visit(nth::Add *add) {
  nodes[add] = "+";
}

void AstDotPrinter::visit(nth::Subtract *subtract) {
  nodes[subtract] = "-";
}

void AstDotPrinter::visit(nth::Multiply *multiply) {
  nodes[multiply] = "*";
}

void AstDotPrinter::visit(nth::Divide *divide) {
  nodes[divide] = "/";
}

void AstDotPrinter::visit(nth::Exponentiate *exp) {
  nodes[exp] = "^";
}

void AstDotPrinter::visit(nth::Modulo *modulo) {
  nodes[modulo] = "%";
}

void AstDotPrinter::visit(nth::BitShiftLeft *shift_left) {
  nodes[shift_left] = "<<";
}

void AstDotPrinter::visit(nth::BitShiftRight *shift_right) {
  nodes[shift_right] = ">>";
}

void AstDotPrinter::visit(nth::BitwiseOr *bitwise_or) {
  nodes[bitwise_or] = "|";
}

void AstDotPrinter::visit(nth::BitwiseAnd *bitwise_and) {
  nodes[bitwise_and] = "&";
}

void AstDotPrinter::visit(nth::BitwiseNot *bitwise_not) {
  nodes[bitwise_not] = "~";
}

void AstDotPrinter::visit(nth::LogicalOr *logical_or) {
  nodes[logical_or] = "||";
}

void AstDotPrinter::visit(nth::LogicalAnd *logical_and) {
  nodes[logical_and] = "&&";
}

void AstDotPrinter::visit(nth::LogicalNot *logical_not) {
  nodes[logical_not] = "!";
}

void AstDotPrinter::visit(nth::Range *range) {
  nodes[range] = "range";

  edges.push_back(std::make_pair(range, range->getStart()));
  edges.push_back(std::make_pair(range, range->getEnd()));

  range->getStart()->accept(*this);
  range->getEnd()->accept(*this);
}

void AstDotPrinter::visit(nth::Tuple *tuple) {
  nodes[tuple] = "tuple";

  for (auto value : tuple->getValues()) {
    edges.push_back(std::make_pair(tuple, value));
    value->accept(*this);
  }
}

void AstDotPrinter::visit(nth::Comparison *comparison) {
  std::string type;

  switch (comparison->getType()) {
    case nth::Comparison::Type::Equality:
      type = "==";
      break;
    case nth::Comparison::Type::Inequality:
      type = "!=";
      break;
    case nth::Comparison::Type::LessThan:
      type = "<";
      break;
    case nth::Comparison::Type::GreaterThan:
      type = ">";
      break;
    case nth::Comparison::Type::LessThanOrEqualTo:
      type = "<=";
      break;
    case nth::Comparison::Type::GreaterThanOrEqualTo:
      type = ">=";
      break;
  }

  nodes[comparison] = type;
}

void AstDotPrinter::visit(nth::Subscript *subscript) {

}
void AstDotPrinter::visit(nth::TupleFieldAccess *field_access) {

}

void AstDotPrinter::visit(nth::FunctionDef *functionDef) {
}

void AstDotPrinter::visit(nth::Argument *argument) {

}

void AstDotPrinter::visit(nth::SimpleType *type) {}
void AstDotPrinter::visit(nth::TemplatedType *type) {}

