#ifndef __ASSIGN_EXPR__
#define __ASSIGN_EXPR__

#include <Lex/Lex.h>
#include <Parse/Expr/Expr.h>
#include <Parse/Expr/ExprVisitor.h>
#include <any>
#include <memory>

namespace arsenic {

class AssignExpr : public Expr {

  Token name;
  std::unique_ptr<Expr> value;

public:
  AssignExpr(Token name, std::unique_ptr<Expr> value);
  std::any accept(ExprVisitor<std::any> &visitor);
  Token getToken() const;
  Expr &getValue() const;
};

} // namespace arsenic

#endif // !__ASSIGN_EXPR__
