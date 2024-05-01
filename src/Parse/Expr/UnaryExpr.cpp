#include <Parse/Expr/UnaryExpr.h>
#include <memory>

namespace arsenic {

UnaryExpr::UnaryExpr(Token op, std::unique_ptr<Expr> expr)
    : operator_t(op), expression(std::move(expr)) {}

std::any UnaryExpr::accept(const ExprVisitor<std::any> &visitor) const {
  return visitor.visit(*this);
}

Token UnaryExpr::getOpLexeme() const { return operator_t; }

const std::unique_ptr<Expr>& UnaryExpr::getExpression() const { return expression; }

} // namespace arsenic
