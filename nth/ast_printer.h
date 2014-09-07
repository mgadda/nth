
#ifndef __nth__ast_printer__
#define __nth__ast_printer__

#include <string>

#include "ast_visitor.h"

class AstPrinter : public nth::Visitor {
public:
  virtual std::string getOutput()=0;
};

#endif /* defined(__nth__ast_printer__) */
