#ifndef __nth__ast_dot_printer__
#define __nth__ast_dot_printer__

#include <string>
#include <sstream>
#include <vector>
#include <map>

#include "ast_visitor.h"

class AstDotPrinter : public nth::Visitor {
public:
  AstDotPrinter();
  
  void visit(nth::Block *block);
  void visit(nth::String *string);
  void visit(nth::Integer *integer);
  void visit(nth::Float *flt);
  void visit(nth::True *tru);
  void visit(nth::False *flse);
  void visit(nth::Identifier *ident);
  void visit(nth::Array *array);
  void visit(nth::Map *map);
  void visit(nth::BinaryOperation *bin_op);
  void visit(nth::UnaryOperation *un_op);
  void visit(nth::Add *add);
  void visit(nth::Subtract *subtract);
  void visit(nth::Multiply *multiply);
  void visit(nth::Divide *divide);
  void visit(nth::Exponentiate *exp);
  void visit(nth::Modulo *modulo);
  void visit(nth::BitShiftLeft *shift_left);
  void visit(nth::BitShiftRight *shift_right);
  void visit(nth::BitwiseOr *bitwise_or);
  void visit(nth::BitwiseAnd *bitwise_and);
  void visit(nth::LogicalOr *logical_or);
  void visit(nth::LogicalAnd *logical_and);
  void visit(nth::LogicalNot *logical_not);
  void visit(nth::Range *range);
  void visit(nth::Tuple *tuple);
  void visit(nth::Comparison *comparison);
  
  
  std::string getOutput();
protected:
  bool pretty_print;
  std::stringstream ast_output;
  
  typedef std::vector<std::pair<nth::Expression*, nth::Expression*>> EdgeList;
  typedef std::map<nth::Expression*, std::string> NodeMap;
  
  EdgeList edges;
  NodeMap nodes;
};

#endif /* defined(__nth__ast_dot_printer__) */
