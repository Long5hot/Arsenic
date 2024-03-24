#ifndef __BINARYEXPR__
#define __BINARYEXPR__

#include <Lex/lex.h>
#include <Parse/Expr/Expr.h>
#include <Parse/Expr/ExprVisitor.h>
#include <any>
#include <memory>

namespace arsenic {

class BinaryExpr {
  std::unique_ptr<Expr> left;
  std::unique_ptr<Expr> right;
  Token operator_t;

public:
  BinaryExpr(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right)
      : left(std::move(left)), operator_t(op), right(std::move(right)) {}

  std::any accept(const ExprVisitor<std::any> &visitor) override {
    return visitor.visit(*this);
  }
};
} // namespace arsenic

#endif
