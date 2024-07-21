
#include <Parse/Expr/CallExpr.h>

namespace arsenic {

CallExpr::CallExpr(std::shared_ptr<Expr> callee, Token cParentheses,
                   std::vector<std::shared_ptr<Expr>> arguments)
    : callee(std::move(callee)), closingParentheses(cParentheses),
      arguments(std::move(arguments)) {}

std::any CallExpr::accept(ExprVisitor<std::any> &visitor) {
  return visitor.visit(*this);
}

Token CallExpr::getClosingParentheses() const { return closingParentheses; }

Expr &CallExpr::getCallee() { return *callee; }

std::vector<std::shared_ptr<Expr>> &CallExpr::getArguments() {
  return arguments;
}

} // namespace arsenic
