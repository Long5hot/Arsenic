#ifndef __VARSTMT__
#define __VARSTMT__

#include <Lex/Lex.h>
#include <Parse/Expr/Expr.h>
#include <Parse/Stmt/Stmt.h>
#include <Parse/Stmt/StmtVisitor.h>
#include <memory>

namespace arsenic {

class VarStmt : public Stmt {

  std::shared_ptr<Expr> initializerExpr;
  Token name;

public:
  VarStmt(Token name, std::shared_ptr<Expr> initializer);

  std::any accept(StmtVisitor<std::any> &visitor) override;

  bool hasInitializer();

  Expr &getInitializer();

  Token getToken();
};

} // namespace arsenic

#endif // !__VARSTMT__
