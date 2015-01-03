#ifndef __nth__scope_checker__
#define __nth__scope_checker__

#include <stack>
#include <functional>
#include "ast_visitor.h"
#include "symbol_table.h"

/*
goals:

convert unique pointers to identifiers into set of symbols
in:
def foo(x: Int) {
  x + 1
}
parsing produces two Identifier instances with a name of "x"
this traversal should collapse instances of the same name into
the same symbol, in addition to verifying that there are no
references to unknown specifiers

algorithm:

encounter something that introduces new identifiers (val, def, lambda)
push them onto the stack
keep track of how many things were pushed
visit sub-nodes
pop same number of things

encounter an identifier
look it up in symbol table
if it exists, attach symbol to identifier
if not, ERROR!

problem: Identifier is used to describe ANY identifier
not just variable references
need to differentiate here
for example

val a: Foo = 10

val_def
  |- ident(a)
  |- simple_type
  |   |- ident(Foo)
  |- integer(10)

In this example, val_def's first child introduces a new
identifier and in turn a new symbol

two questions:
is it possible that types aren't really very different
from variable references?
*/

namespace nth {

typedef std::list<nth::Identifier*> IdentifierList;

class ASTNode;

class ScopeChecker {
 public:
  ScopeChecker();
  ScopeChecker(SymbolTable &table);

  bool run(ASTNode *node);

  bool isValid();
  IdentifierList &getUnknownIdentifiers();
 protected:
  void withNewScope(ASTNode *node, std::function<void(SymbolTable&)> scopedFun);
  void withScope(nth::ASTNode *node, std::function<void(SymbolTable&)>scopedFun);
  std::stack<SymbolTable*> symbolTables;
  IdentifierList unknownIdentifiers;

  // Returns table attached to node, or top of stack
  SymbolTable &getSymbolTable(nth::ASTNode *node);

  // Responsible for building preliminary symbol tables, attaching them when
  // appropriate to ASTNodes. The tables contain symbols defined are those
  // which are not required to be declaraed before usage.
  class ScopePrimer : public Visitor {
   public:
    ScopePrimer(ScopeChecker &checker);

    void visit(nth::Block *block);
    void visit(nth::FunctionDef *functionDef);
    void visit(nth::LambdaDef *lambdaDef);
    void visit(nth::TypeAliasDef *typeAliasDef);
    void visit(nth::TraitDef *traitDef);

   protected:
    ScopeChecker &scopeChecker;
  };

  //
  class DeclareBeforeUse : public Visitor {
   public:
    DeclareBeforeUse(ScopeChecker &checker);

    void visit(nth::Block *block);
    void visit(nth::Identifier *ident);
    void visit(nth::FieldAccess *field_access);
    void visit(nth::FunctionDef *functionDef);
    void visit(nth::LambdaDef *lambdaDef);
    void visit(nth::VariableDef *variableDef);
    void visit(nth::Argument *argument);

    void visit(nth::SimpleTypeRef *type);
    void visit(nth::SimpleTypeDef *type);
    void visit(nth::TemplatedTypeRef *type);
    void visit(nth::TemplatedTypeDef *type);

    void visit(nth::TypeAliasDef *typeAliasDef);

   protected:
    ScopeChecker &scopeChecker;

  };
};

}

#endif
