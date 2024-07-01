#include <Parse/Stmt/VarStmt.h>

namespace arsenic {

VarStmt::VarStmt(Token name, std::unique_ptr<Expr> initializer)
    : name(name), initializerExpr(std::move(initializer)) {}

std::any VarStmt::accept(StmtVisitor<std::any> &visitor) {
  return visitor.visit(*this);
}

Expr &VarStmt::getInitializer() { return *initializerExpr; }

Token VarStmt::getToken() { return name; }

bool VarStmt::hasInitializer() { return initializerExpr != nullptr; }

} // namespace arsenic
