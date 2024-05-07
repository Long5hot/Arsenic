#ifndef __EXPR__
#define __EXPR__

#include <Parse/Expr/ExprVisitor.h>
#include <any>

namespace arsenic {

class Expr {

public:
  virtual ~Expr() = default;

  virtual std::any accept(ExprVisitor<std::any> &) = 0;
};
} // namespace arsenic

#endif
