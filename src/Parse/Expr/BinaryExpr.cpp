
#include <Parse/Expr/BinaryExpr.h>

namespace arsenic {

BinaryExpr::BinaryExpr(std::unique_ptr<Expr> left, Token op,
                       std::unique_ptr<Expr> right)
    : left(std::move(left)), operator_t(op), right(std::move(right)) {}

std::any BinaryExpr::accept(const ExprVisitor<std::any> &visitor) const {
  return visitor.visit(*this);
}

Token BinaryExpr::getOpToken() const { return operator_t; }

const std::unique_ptr<Expr>& BinaryExpr::getLeftExpr() const { return left; }

const std::unique_ptr<Expr>& BinaryExpr::getRightExpr() const { return right; }

} // namespace arsenic
