#include <Parse/Stmt/PrintStmt.h>

namespace arsenic {

PrintStmt::PrintStmt(std::unique_ptr<Expr> expr)
    : expression(std::move(expr)) {}

std::any PrintStmt::accept(StmtVisitor<std::any> &visitor) {
  return visitor.visit(*this);
}

std::unique_ptr<Expr> &PrintStmt::getExpr() { return expression; }

} // namespace arsenic
