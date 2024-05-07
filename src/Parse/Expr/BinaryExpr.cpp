
#include <Parse/Expr/BinaryExpr.h>

namespace arsenic {

BinaryExpr::BinaryExpr(std::unique_ptr<Expr> left, Token op,
                       std::unique_ptr<Expr> right)
    : left(std::move(left)), operator_t(op), right(std::move(right)) {}

std::any BinaryExpr::accept(ExprVisitor<std::any> &visitor) {
  return visitor.visit(*this);
}

Token BinaryExpr::getOpToken() const { return operator_t; }

std::unique_ptr<Expr>& BinaryExpr::getLeftExpr() { return left; }

std::unique_ptr<Expr>& BinaryExpr::getRightExpr() { return right; }

} // namespace arsenic
