#ifndef __nth__symbol_table__
#define __nth__symbol_table__

#include <deque>
#include <stack>
#include <string>

namespace nth {

  class Symbol;
  class Identifier;

  class SymbolTable {
  public:
    typedef std::deque<nth::Symbol*> Scope;

    void pushScope();
    void popScope();

    Symbol *findSymbol(Identifier *ident);
    void addSymbol(Identifier *ident);

    // Check if symbol is defined in current frame only
    bool checkSymbol(Identifier *ident);

    std::deque<nth::Symbol*> getSymbols();
    std::deque<Scope> &getScopes();
  protected:
    std::deque<Scope> scopes;
  };

  class Symbol {
  public:
    Symbol(Identifier *ident);
    bool operator==(Identifier *ident);
    bool operator==(std::string str);
    operator std::string() const { return name; }
    operator const char *() const { return name.c_str(); }
  protected:
    std::string name;
  };
  
}

#endif

