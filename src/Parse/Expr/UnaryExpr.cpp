#include <Parse/Expr/UnaryExpr.h>
#include <memory>

namespace arsenic {

UnaryExpr::UnaryExpr(Token op, std::unique_ptr<Expr> expr)
    : operator_t(op), expression(std::move(expr)) {}

std::any UnaryExpr::accept(ExprVisitor<std::any> &visitor) {
  return visitor.visit(*this);
}

Token UnaryExpr::getOpLexeme() const { return operator_t; }

Expr &UnaryExpr::getExpression() { return *expression; }

} // namespace arsenic
