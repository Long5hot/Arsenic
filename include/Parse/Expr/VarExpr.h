#ifndef __VAREXPR__
#define __VAREXPR__

#include <Lex/Lex.h>
#include <Parse/Expr/Expr.h>
#include <Parse/Expr/ExprVisitor.h>
#include <any>

namespace arsenic {

class VarExpr : public Expr {
  Token name;

public:
  VarExpr(Token name);

  std::any accept(ExprVisitor<std::any> &visitor) override;

  Token getToken() const;
};
} // namespace arsenic

#endif
