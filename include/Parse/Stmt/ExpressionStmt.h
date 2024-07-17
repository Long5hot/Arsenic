#ifndef __EXPRSTMT__
#define __EXPRSTMT__

#include <Parse/Expr/Expr.h>
#include <Parse/Stmt/Stmt.h>
#include <any>
#include <memory>

namespace arsenic {

class ExpressionStmt : public Stmt {
  std::shared_ptr<Expr> expression;

public:
  ExpressionStmt(std::shared_ptr<Expr> expr);

  std::any accept(StmtVisitor<std::any> &visitor) override;

  Expr &getExpr();
};

} // namespace arsenic

#endif // __EXPRSTMT__
