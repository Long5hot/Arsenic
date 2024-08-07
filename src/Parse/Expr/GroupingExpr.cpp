#include <Parse/Expr/GroupingExpr.h>
#include <memory>
#include <mutex>
#include <random>

namespace arsenic {

GroupingExpr::GroupingExpr(std::shared_ptr<Expr> expression)
    : expression(std::move(expression)) {}

std::any GroupingExpr::accept(ExprVisitor<std::any> &visitor) {
  return visitor.visit(*this);
}

Expr &GroupingExpr::getExpression() { return *expression; }

} // namespace arsenic
