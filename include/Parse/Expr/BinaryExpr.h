#ifndef __BINARYEXPR__
#define __BINARYEXPR__

#include <Lex/Lex.h>
#include <Parse/Expr/Expr.h>
#include <Parse/Expr/ExprVisitor.h>
#include <any>
#include <memory>

namespace arsenic {

class BinaryExpr : public Expr {
  std::unique_ptr<Expr> left;
  std::unique_ptr<Expr> right;
  Token operator_t;

public:
  BinaryExpr(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right);

  std::any accept(const ExprVisitor<std::any> &visitor) const override;

  Token getOpToken() const;

  const std::unique_ptr<Expr>& getLeftExpr() const;

  const std::unique_ptr<Expr>& getRightExpr() const;
};
} // namespace arsenic

#endif
