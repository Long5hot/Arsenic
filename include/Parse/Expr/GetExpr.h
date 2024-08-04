#ifndef __GETEXPR__
#define __GETEXPR__

#include <Lex/Lex.h>
#include <Parse/Expr/Expr.h>
#include <Parse/Expr/ExprVisitor.h>
#include <memory>

namespace arsenic {

class GetExpr : public Expr {

  Token name;
  std::shared_ptr<Expr> object;

public:
  GetExpr(Token name, std::shared_ptr<Expr> object);

  std::any accept(ExprVisitor<std::any> &visitor) override;

  Token getToken() const;

  Expr &getObject();

  std::shared_ptr<Expr> getObjectPtr();
};

} // namespace arsenic

#endif // __GETEXPR__
