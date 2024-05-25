#ifndef __UNARY_EXPR__
#define __UNARY_EXPR__

#include <Lex/Lex.h>
#include <Parse/Expr/Expr.h>
#include <Parse/Expr/ExprVisitor.h>
#include <memory>

namespace arsenic {

class UnaryExpr : public Expr {

  Token operator_t;
  std::unique_ptr<Expr> expression;

public:
  UnaryExpr(Token op, std::unique_ptr<Expr> expr);

  std::any accept(ExprVisitor<std::any> &visitor) override;

  Token getOpLexeme() const;

  std::unique_ptr<Expr> &getExpression();
};

} // namespace arsenic

#endif
