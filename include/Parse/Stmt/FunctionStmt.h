#ifndef __FUNCTIONSTMT__
#define __FUNCTIONSTMT__

#include <Lex/Lex.h>
#include <Parse/Expr/Expr.h>
#include <Parse/Stmt/Stmt.h>
#include <Parse/Stmt/StmtVisitor.h>
#include <memory>

namespace arsenic {

class FunctionStmt : public Stmt {

  Token name;
  std::vector<Token> parameters;
  std::vector<std::shared_ptr<Stmt>> functionBody;

public:
  FunctionStmt(Token name, std::vector<Token>,
               std::vector<std::shared_ptr<Stmt>> functionBody);
  FunctionStmt(FunctionStmt &);
  std::any accept(StmtVisitor<std::any> &visitor) override;
  int parameterSize();
  std::vector<Token> getParameters();
  Token getToken();
  std::vector<std::shared_ptr<Stmt>> &getFunctionBody();
};

} // namespace arsenic

#endif // !__VARSTMT__
