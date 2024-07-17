#ifndef __BLOCKSTMT__
#define __BLOCKSTMT__

#include <Parse/Expr/Expr.h>
#include <Parse/Stmt/Stmt.h>
#include <Parse/Stmt/StmtVisitor.h>
#include <memory>
#include <vector>

namespace arsenic {

class BlockStmt : public Stmt {
  std::vector<std::shared_ptr<Stmt>> statements;

public:
  BlockStmt(std::vector<std::shared_ptr<Stmt>> stmts);

  std::any accept(StmtVisitor<std::any> &visitor) override;

  std::vector<std::shared_ptr<Stmt>> &getStatements();
};

} // namespace arsenic

#endif // !__BLOCKSTMT__
