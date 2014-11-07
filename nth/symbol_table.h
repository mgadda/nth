#ifndef __nth__symbol_table__
#define __nth__symbol_table__

#include <deque>
#include <string>
#include <list>

namespace nth {

  class Symbol;
  class Identifier;

  class SymbolTable {
  public:
    SymbolTable();
    SymbolTable(SymbolTable *parent);
    SymbolTable(std::initializer_list<Identifier*> identifierList);
    // Return new child table
    SymbolTable *beget();

    // Search this scope and parents for this identifier
    Symbol *findSymbol(Identifier *ident);
    void addSymbol(Identifier *ident);

    // Check if Identifier is defined in current frame only
    bool checkSymbol(Identifier *ident);

    std::deque<nth::Symbol*> getSymbols();
    std::list<SymbolTable *> getScopes();
  protected:
    std::deque<nth::Symbol*> scope;

    Symbol *findLocalSymbol(Identifier *ident);
    SymbolTable *parent;

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

