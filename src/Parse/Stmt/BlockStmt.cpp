#include <Parse/Stmt/BlockStmt.h>

namespace arsenic {

BlockStmt::BlockStmt(std::vector<std::shared_ptr<Stmt>> stmts)
    : statements(std::move(stmts)) {}

std::any BlockStmt::accept(StmtVisitor<std::any> &visitor) {
  return visitor.visit(*this);
}

std::vector<std::shared_ptr<Stmt>> &BlockStmt::getStatements() {
  return statements;
}

} // namespace arsenic
