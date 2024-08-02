#include <Parse/Expr/GetExpr.h>
#include <memory>

namespace arsenic {

GetExpr::GetExpr(Token name, std::shared_ptr<Expr> object)
    : name(name), object(std::move(object)) {}

std::any GetExpr::accept(ExprVisitor<std::any> &visitor) {
  return visitor.visit(*this);
}

Token GetExpr::getToken() const { return name; }

Expr &GetExpr::getObject() { return *object; }

} // namespace arsenic
