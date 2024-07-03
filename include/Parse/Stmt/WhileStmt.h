#ifndef __WHILESTMT__
#define __WHILESTMT__

#include <Parse/Expr/Expr.h>
#include <Parse/Stmt/Stmt.h>
#include <Parse/Stmt/StmtVisitor.h>
#include <memory>

namespace arsenic {

class WhileStmt : public Stmt {

  std::unique_ptr<Expr> condition;
  std::unique_ptr<Stmt> whileBody;

public:
  WhileStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> whileBody);

  std::any accept(StmtVisitor<std::any> &visitor) override;

  Expr &getCondition();

  Stmt &getWhileBody();
};

} // namespace arsenic

#endif // !__WHILESTMT__
