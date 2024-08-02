#include <Parse/Stmt/ClassStmt.h>

namespace arsenic {

ClassStmt::ClassStmt(Token name, std::vector<std::shared_ptr<Stmt>> methods)
    : name(name), methods(std::move(methods)) {}

std::any ClassStmt::accept(StmtVisitor<std::any> &visitor) {
  return visitor.visit(*this);
}

Token ClassStmt::getToken() { return name; }

std::vector<std::shared_ptr<Stmt>> &ClassStmt::getClassBody() {
  return methods;
}

} // namespace arsenic
