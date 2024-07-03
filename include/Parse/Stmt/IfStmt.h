#ifndef __IFSTMT__
#define __IFSTMT__

#include <Parse/Expr/Expr.h>
#include <Parse/Stmt/Stmt.h>
#include <Parse/Stmt/StmtVisitor.h>
#include <memory>

namespace arsenic {

class IfStmt : public Stmt {

  std::unique_ptr<Expr> condition;
  std::unique_ptr<Stmt> thenBranch;
  std::unique_ptr<Stmt> elseBranch;

public:
  IfStmt(std::unique_ptr<Expr> expr, std::unique_ptr<Stmt> thenBranch,
         std::unique_ptr<Stmt> elseBranch);

  std::any accept(StmtVisitor<std::any> &visitor) override;

  Expr &getCondition();

  Stmt &getThenBranch();

  Stmt &getElseBranch();

  bool hasElseBranch();
};

} // namespace arsenic

#endif // !__IFSTMT__
