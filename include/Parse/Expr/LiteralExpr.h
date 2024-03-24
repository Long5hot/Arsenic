#ifndef __LITERAL_EXPR__
#define __LITERAL_EXPR__

#include <Parse/Expr/Expr.h>
#include <Parse/Expr/ExprVisitor.h>
#include <any>

namespace arsenic {

class LiteralExpr : public Expr {

  std::any literal_value;

public:
  LiteralExpr(std::any value_t) : literal_value(value_t) {}

  std::any accept(const ExprVisitor<std::any> &visitor) const override {
    return visitor.visit(*this);
  }
};

} // namespace arsenic
#endif
