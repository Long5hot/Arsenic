#include <Parse/Stmt/ReturnStmt.h>
#include <memory>

namespace arsenic {

ReturnStmt::ReturnStmt(Token keyword, std::shared_ptr<Expr> value)
    : keyword(keyword), value(std::move(value)) {}

std::any ReturnStmt::accept(StmtVisitor<std::any> &visitor) {
  return visitor.visit(*this);
}

std::shared_ptr<Expr> ReturnStmt::getExpressionValue() { return value; }

Token ReturnStmt::getToken() const { return keyword; }

} // namespace arsenic
