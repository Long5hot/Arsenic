#ifndef __BINARYEXPR__
#define __BINARYEXPR__

#include <Lex/Lex.h>
#include <Parse/Expr/Expr.h>
#include <Parse/Expr/ExprVisitor.h>
#include <any>
#include <memory>

namespace arsenic {

class BinaryExpr : public Expr {
  std::shared_ptr<Expr> left;
  std::shared_ptr<Expr> right;
  Token operator_t;

public:
  BinaryExpr(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right);

  std::any accept(ExprVisitor<std::any> &visitor) override;

  Token getOpToken() const;

  Expr &getLeftExpr();

  Expr &getRightExpr();
};
} // namespace arsenic

#endif
