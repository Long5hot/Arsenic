#ifndef __EXPRSTMT__
#define __EXPRSTMT__

#include <Parse/Expr/Expr.h>
#include <Parse/Stmt/Stmt.h>
#include <any>
#include <memory>

namespace arsenic {

class ExpressionStmt : public Stmt {
  std::unique_ptr<Expr> expression;

public:
  ExpressionStmt(std::unique_ptr<Expr> expr);

  std::any accept(StmtVisitor<std::any> &visitor) override;

  std::unique_ptr<Expr> &getExpr();
};

} // namespace arsenic

#endif // __EXPRSTMT__
