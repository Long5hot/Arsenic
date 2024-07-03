#ifndef __LITERAL_EXPR__
#define __LITERAL_EXPR__

#include <Parse/Expr/Expr.h>
#include <Parse/Expr/ExprVisitor.h>
#include <any>
#include <string>

namespace arsenic {

class LiteralExpr : public Expr {

  std::any literal_value;

public:
  LiteralExpr();

  LiteralExpr(std::any value_t);

  std::any accept(ExprVisitor<std::any> &visitor) override;

  std::string getLiteralStringValue() const;

  std::any getLiteralValue() const;
};

} // namespace arsenic

#endif
