#ifndef __UNARY_EXPR__
#define __UNARY_EXPR__

#include <Lex/Lex.h>
#include <Parse/Expr/Expr.h>
#include <Parse/Expr/ExprVisitor.h>
#include <memory>

namespace arsenic {

class UnaryExpr : public Expr {

  Token operator_t;
  std::shared_ptr<Expr> expression;

public:
  UnaryExpr(Token op, std::shared_ptr<Expr> expr);

  std::any accept(ExprVisitor<std::any> &visitor) override;

  Token getOpLexeme() const;

  Expr &getExpression();
};

} // namespace arsenic

#endif
