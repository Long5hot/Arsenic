#include <Parse/Expr/SetExpr.h>
#include <memory>

namespace arsenic {

SetExpr::SetExpr(Token Name, std::shared_ptr<Expr> Object,
                 std::shared_ptr<Expr> Value)
    : Name(Name), Object(std::move(Object)), Value(std::move(Value)) {}

std::any SetExpr::accept(ExprVisitor<std::any> &visitor) {
  return visitor.visit(*this);
}

Token SetExpr::getToken() const { return Name; }

Expr &SetExpr::getObject() { return *Object; }

Expr &SetExpr::getValue() { return *Value; }

} // namespace arsenic
