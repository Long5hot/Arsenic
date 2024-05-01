#ifndef __GROUPING_EXPR_H__
#define __GROUPING_EXPR_H__

#include <Parse/Expr/Expr.h>
#include <any>
#include <memory>

namespace arsenic {

class GroupingExpr : public Expr {

  std::unique_ptr<Expr> expression;

public:
  GroupingExpr(std::unique_ptr<Expr> expression);

  std::any accept(const ExprVisitor<std::any>& visitor) const override;

  const std::unique_ptr<Expr>& getExpression() const;
};

} // namespace arsenic

#endif
