#ifndef __PRINTSTMT__
#define __PRINTSTMT__

#include <Parse/Expr/Expr.h>
#include <Parse/Stmt/Stmt.h>
#include <Parse/Stmt/StmtVisitor.h>
#include <memory>

namespace arsenic {

class PrintStmt : public Stmt {
  std::shared_ptr<Expr> expression;

public:
  PrintStmt(std::shared_ptr<Expr> expr);

  std::any accept(StmtVisitor<std::any> &visitor) override;

  Expr &getExpr();
};

} // namespace arsenic

#endif // !__PRINTSTMT__
