#ifndef __CALLEXPR__
#define __CALLEXPR__

#include <Lex/Lex.h>
#include <Parse/Expr/Expr.h>
#include <Parse/Expr/ExprVisitor.h>
#include <any>
#include <memory>
#include <vector>

namespace arsenic {

class CallExpr : public Expr {
  std::shared_ptr<Expr> callee;
  std::vector<std::shared_ptr<Expr>> arguments;
  Token closingParentheses;

public:
  CallExpr(std::shared_ptr<Expr> callee, Token cParentheses,
           std::vector<std::shared_ptr<Expr>> arguments);

  std::any accept(ExprVisitor<std::any> &visitor) override;

  Token getClosingParentheses() const;

  Expr &getCallee();

  std::vector<std::shared_ptr<Expr>> &getArguments();
};
} // namespace arsenic

#endif
