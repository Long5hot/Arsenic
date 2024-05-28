
#include <Parse/Expr/VarExpr.h>

namespace arsenic {

VarExpr::VarExpr(Token name) : name(name) {}

std::any VarExpr::accept(ExprVisitor<std::any> &visitor) {
  return visitor.visit(*this);
}

Token VarExpr::getToken() const {
  return name;
}

} // namespace arsenic
