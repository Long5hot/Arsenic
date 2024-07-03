#include <Parse/Stmt/WhileStmt.h>

namespace arsenic {

WhileStmt::WhileStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> whileBody) :
  condition(std::move(condition)), whileBody(std::move(whileBody)) {}

std::any WhileStmt::accept(StmtVisitor<std::any> &visitor) {
  return visitor.visit(*this);
}

Expr &WhileStmt::getCondition() { return *condition; }

Stmt &WhileStmt::getWhileBody() { return *whileBody; }

} // namespace arsenic
