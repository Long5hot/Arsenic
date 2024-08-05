#include <Parse/Stmt/ClassStmt.h>
#include <memory>

namespace arsenic {

ClassStmt::ClassStmt(Token name, std::shared_ptr<VarExpr> SuperClass,
                     std::vector<std::shared_ptr<Stmt>> methods)
    : name(name), SuperClass(std::move(SuperClass)),
      methods(std::move(methods)) {}

std::any ClassStmt::accept(StmtVisitor<std::any> &visitor) {
  return visitor.visit(*this);
}

Token ClassStmt::getToken() { return name; }

std::vector<std::shared_ptr<Stmt>> &ClassStmt::getMethods() { return methods; }

std::shared_ptr<VarExpr> ClassStmt::getSuperClass() { return SuperClass; }

} // namespace arsenic
