#include <Parse/Expr/AssignExpr.h>
#include <memory>

namespace arsenic {

AssignExpr::AssignExpr(Token name, std::shared_ptr<Expr> value)
    : name(name), value(std::move(value)) {}

std::any AssignExpr::accept(ExprVisitor<std::any> &visitor) {
  return visitor.visit(*this);
}

bool AssignExpr::operator==(const AssignExpr & obj) const {
  return this->name.getLiteralValue() == obj.getToken().getLiteralValue();
}

Token AssignExpr::getToken() const { return name; }

Expr &AssignExpr::getValue() const { return *value; }

} // namespace arsenic
