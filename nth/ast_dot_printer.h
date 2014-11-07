#ifndef __nth__ast_dot_printer__
#define __nth__ast_dot_printer__

#include <string>
#include <sstream>
#include <vector>
#include <map>

#include "ast_printer.h"

namespace nth {
class ASTNode;

class AstDotPrinter : public AstPrinter {
public:
  AstDotPrinter();

  void visit(Block *block);
  void visit(String *string);
  void visit(Integer *integer);
  void visit(Float *flt);
  void visit(True *tru);
  void visit(False *flse);
  void visit(Identifier *ident);
  void visit(Array *array);
  void visit(Map *map);
  void visit(BinaryOperation *bin_op);
  void visit(UnaryOperation *un_op);
  void visit(Add *add);
  void visit(Subtract *subtract);
  void visit(Multiply *multiply);
  void visit(Divide *divide);
  void visit(Exponentiate *exp);
  void visit(Modulo *modulo);
  void visit(BitShiftLeft *shift_left);
  void visit(BitShiftRight *shift_right);
  void visit(BitwiseOr *bitwise_or);
  void visit(BitwiseAnd *bitwise_and);
  void visit(BitwiseNot *bitwise_not);
  void visit(LogicalOr *logical_or);
  void visit(LogicalAnd *logical_and);
  void visit(LogicalNot *logical_not);
  void visit(Range *range);
  void visit(Tuple *tuple);
  void visit(Comparison *comparison);
  void visit(Subscript *subscript);
  void visit(FieldAccess *field_access);
  void visit(TupleFieldAccess *tuple_field_access);
  void visit(FunctionDef *functionDef);
  void visit(LambdaDef *lambdaDef);
  void visit(FunctionCall *functionCall);
  void visit(VariableDef *variableDef);
  void visit(Argument *argument);
  void visit(IfElse *ifElse);

  void visit(SimpleTypeRef *type);
  void visit(SimpleTypeDef *type);
  void visit(TemplatedTypeRef *type);
  void visit(TemplatedTypeDef *type);
  void visit(TypeAliasDef *typeAliasDef);

  std::string getOutput();
protected:
  DummyNode *makeDummy(std::string name, ASTNode *parent);

  bool pretty_print;
  std::stringstream ast_output;

  typedef std::vector<std::pair<ASTNode*, ASTNode*>> EdgeList;
  typedef std::map<ASTNode*, std::string> NodeMap;

  EdgeList edges;
  NodeMap nodes;

};

}

#endif /* defined(__nth__ast_dot_printer__) */
