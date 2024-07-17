#ifndef __GROUPING_EXPR_H__
#define __GROUPING_EXPR_H__

#include <Parse/Expr/Expr.h>
#include <any>
#include <memory>

namespace arsenic {

class GroupingExpr : public Expr {

  std::shared_ptr<Expr> expression;

public:
  GroupingExpr(std::shared_ptr<Expr> expression);

  std::any accept(ExprVisitor<std::any> &visitor) override;

  Expr &getExpression();
};

} // namespace arsenic

#endif
