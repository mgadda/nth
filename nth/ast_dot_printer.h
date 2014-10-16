#ifndef __nth__ast_dot_printer__
#define __nth__ast_dot_printer__

#include <string>
#include <sstream>
#include <vector>
#include <map>

#include "ast_printer.h"

class AstDotPrinter : public AstPrinter {
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
  void visit(nth::BitwiseNot *bitwise_not);
  void visit(nth::LogicalOr *logical_or);
  void visit(nth::LogicalAnd *logical_and);
  void visit(nth::LogicalNot *logical_not);
  void visit(nth::Range *range);
  void visit(nth::Tuple *tuple);
  void visit(nth::Comparison *comparison);
  void visit(nth::Subscript *subscript);
  void visit(nth::FieldAccess *field_access);
  void visit(nth::TupleFieldAccess *tuple_field_access);
  void visit(nth::FunctionDef *functionDef);
  void visit(nth::LambdaDef *lambdaDef);
  void visit(nth::FunctionCall *functionCall);
  void visit(nth::VariableDef *variableDef);
  void visit(nth::Argument *argument);
  void visit(nth::IfElse *ifElse);

  void visit(nth::SimpleTypeRef *type);
  void visit(nth::SimpleTypeDef *type);
  void visit(nth::TemplatedTypeRef *type);
  void visit(nth::TemplatedTypeDef *type);
  void visit(nth::TypeAliasDef *typeAliasDef);

  std::string getOutput();
protected:
  bool pretty_print;
  std::stringstream ast_output;

  typedef std::vector<std::pair<nth::ASTNode*, nth::ASTNode*>> EdgeList;
  typedef std::map<nth::ASTNode*, std::string> NodeMap;

  EdgeList edges;
  NodeMap nodes;
};

#endif /* defined(__nth__ast_dot_printer__) */
