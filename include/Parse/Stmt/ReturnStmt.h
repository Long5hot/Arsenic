#ifndef __RETURNSTMT__
#define __RETURNSTMT__

#include <Lex/Lex.h>
#include <Parse/Expr/Expr.h>
#include <Parse/Stmt/Stmt.h>
#include <Parse/Stmt/StmtVisitor.h>
#include <memory>

namespace arsenic {

class ReturnStmt : public Stmt {

  std::shared_ptr<Expr> value;
  Token keyword;

public:
  ReturnStmt(Token keyword, std::shared_ptr<Expr> value);

  std::any accept(StmtVisitor<std::any> &visitor) override;

  std::shared_ptr<Expr> getExpressionValue();

  Token getToken() const;
};

} // namespace arsenic

#endif // !__RETURNSTMT__
