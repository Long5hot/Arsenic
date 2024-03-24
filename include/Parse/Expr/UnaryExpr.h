#ifndef __UNARY_EXPR__
#define __UNARY_EXPR__

#include <Lex/lex.h>
#include <Parse/Expr/Expr.h>
#include <Parse/Expr/ExprVisitor.h>
#include <memory>

namespace arsenic {

class UnaryExpr : public Expr {

  Token operator_t;
  std::unique_ptr<Expr> expression;

public:
  UnaryExpr(Token op, std::unique_ptr<Expr> expr)
      : operator_t(op), expression(std::move(expr)) {}

  std::any accept(const ExprVisitor<std::any> &visitor) const override {
    return visitor.visit(*this);
  }
};

} // namespace arsenic

#endif
