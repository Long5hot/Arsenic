#ifndef __SETEXPR__
#define __SETEXPR__

#include <Lex/Lex.h>
#include <Parse/Expr/Expr.h>
#include <Parse/Expr/ExprVisitor.h>
#include <memory>

namespace arsenic {

class SetExpr : public Expr {

  Token Name;
  std::shared_ptr<Expr> Object;
  std::shared_ptr<Expr> Value;

public:
  SetExpr(Token Name, std::shared_ptr<Expr> Object,
          std::shared_ptr<Expr> Value);

  std::any accept(ExprVisitor<std::any> &visitor) override;

  Token getToken() const;

  Expr &getObject();

  Expr &getValue();
};

} // namespace arsenic

#endif // __SETEXPR__
