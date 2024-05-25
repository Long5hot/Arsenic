#include <Parse/Stmt/ExpressionStmt.h>

namespace arsenic {

ExpressionStmt::ExpressionStmt(std::unique_ptr<Expr> expr)
    : expression(std::move(expr)) {}

std::any ExpressionStmt::accept(StmtVisitor<std::any> &visitor) {
  return visitor.visit(*this);
}

std::unique_ptr<Expr> &ExpressionStmt::getExpr() { return expression; }

} // namespace arsenic
