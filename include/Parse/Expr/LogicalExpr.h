#ifndef __LOGICALEXPR__
#define __LOGICALEXPR__

#include <Lex/Lex.h>
#include <Parse/Expr/Expr.h>
#include <Parse/Expr/ExprVisitor.h>
#include <any>
#include <memory>

namespace arsenic {

class LogicalExpr : public Expr {
  std::unique_ptr<Expr> left;
  std::unique_ptr<Expr> right;
  Token operator_t;

public:
  LogicalExpr(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right);

  std::any accept(ExprVisitor<std::any> &visitor) override;

  Token getOpToken() const;

  Expr &getLeftExpr();

  Expr &getRightExpr();
};

} // namespace arsenic

#endif
