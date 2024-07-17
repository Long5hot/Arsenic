#include <Parse/Stmt/IfStmt.h>

namespace arsenic {

IfStmt::IfStmt(std::shared_ptr<Expr> expr, std::shared_ptr<Stmt> thenBranch,
               std::shared_ptr<Stmt> elseBranch)
    : condition(std::move(expr)), thenBranch(std::move(thenBranch)),
      elseBranch(std::move(elseBranch)) {}

std::any IfStmt::accept(StmtVisitor<std::any> &visitor) {
  return visitor.visit(*this);
}

Expr &IfStmt::getCondition() { return *condition; }

Stmt &IfStmt::getThenBranch() { return *thenBranch; }

Stmt &IfStmt::getElseBranch() { return *elseBranch; }

bool IfStmt::hasElseBranch() { return elseBranch != nullptr; }

} // namespace arsenic
