#include <Parse/Expr/LogicalExpr.h>

namespace arsenic {

LogicalExpr::LogicalExpr(std::shared_ptr<Expr> left, Token op,
                         std::shared_ptr<Expr> right)
    : left(std::move(left)), operator_t(op), right(std::move(right)) {}

std::any LogicalExpr::accept(ExprVisitor<std::any> &visitor) {
  return visitor.visit(*this);
}

Token LogicalExpr::getOpToken() const { return operator_t; }

Expr &LogicalExpr::getLeftExpr() { return *left; }

Expr &LogicalExpr::getRightExpr() { return *right; }

} // namespace arsenic
