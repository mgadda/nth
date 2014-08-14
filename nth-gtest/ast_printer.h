
#ifndef __nth__ast_printer__
#define __nth__ast_printer__

#include <string>
#include <sstream>

#include "ast_visitor.h"

class AstPrinter : public nth::Visitor {
public:
  AstPrinter(bool pretty_print=false) : pretty_print(pretty_print) {}

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
  void visit(nth::BitwiseNot *bitwise_not);
  void visit(nth::LogicalOr *logical_or);
  void visit(nth::LogicalAnd *logical_and);
  void visit(nth::LogicalNot *logical_not);
  void visit(nth::Range *range);
  void visit(nth::Tuple *tuple);
  void visit(nth::Comparison *comparison);
  void visit(nth::Subscript *subscript);
  void visit(nth::TupleFieldAccess *field_access);
  void visit(nth::FunctionDef *functionDef);

  std::string getOutput();
protected:
  bool pretty_print;
  std::stringstream ast_output;
};

// Iterate from first to last and append c onto ss
// between each value in InputIterator
template <class InputIterator, class Function>
void join_values(InputIterator first, InputIterator last, std::string c, std::stringstream &ss, Function f);

#endif /* defined(__nth__ast_printer__) */
