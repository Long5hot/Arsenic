#ifndef __CLASSSTMT__
#define __CLASSSTMT__

#include <Lex/Lex.h>
#include <Parse/Expr/Expr.h>
#include <Parse/Expr/VarExpr.h>
#include <Parse/Stmt/FunctionStmt.h>
#include <Parse/Stmt/Stmt.h>
#include <Parse/Stmt/StmtVisitor.h>

namespace arsenic {

class ClassStmt : public Stmt {

  Token name;
  std::shared_ptr<VarExpr> SuperClass;
  std::vector<std::shared_ptr<Stmt>> methods;

public:
  ClassStmt(Token name, std::shared_ptr<VarExpr> SuperClass,
            std::vector<std::shared_ptr<Stmt>> methods);
  std::any accept(StmtVisitor<std::any> &visitor) override;
  Token getToken();
  std::vector<std::shared_ptr<Stmt>> &getMethods();
  std::shared_ptr<VarExpr> getSuperClass();
};

} // namespace arsenic

#endif // !__CLASSSTMT__
