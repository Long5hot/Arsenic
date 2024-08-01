#ifndef __RESOLVERH__
#define __RESOLVERH__

#include <Error/arsenic_error>
#include <Parse/Interpret.h>
#include <Parse/Expr/DyadExpr.h>

namespace arsenic {

class Resolver : public ExprVisitor<std::any>, public StmtVisitor<std::any> {

  enum FunctionType { NONE, FUNCTION };

  std::shared_ptr<Interpreter> interpreter;
  std::vector<std::shared_ptr<std::unordered_map<std::string, bool>>> Scopes;
  FunctionType CurrentFunction = FunctionType::NONE;

public:
  Resolver(std::shared_ptr<Interpreter> interpreter);

  void BeginScope();
  void EndScope();

  void declare(Token Name);
  void define(Token Name);

  void resolve(Stmt &stmt);
  void resolve(Expr &expr);
  void resolve(std::vector<std::shared_ptr<Stmt>> statements);
  void resolveLocal(Dyad_expr expr, Token Name);
  void resolveFunction(FunctionStmt &function, FunctionType Type);

  std::any visit(LiteralExpr &expr);
  std::any visit(GroupingExpr &expr);
  std::any visit(UnaryExpr &expr);
  std::any visit(BinaryExpr &expr);
  std::any visit(VarExpr &stmt);
  std::any visit(AssignExpr &expr);
  std::any visit(LogicalExpr &expr);
  std::any visit(CallExpr &expr);
  std::any visit(ExpressionStmt &stmt);
  std::any visit(PrintStmt &stmt);
  std::any visit(VarStmt &stmt);
  std::any visit(BlockStmt &stmt);
  std::any visit(IfStmt &stmt);
  std::any visit(WhileStmt &stmt);
  std::any visit(FunctionStmt &stmt);
  std::any visit(ReturnStmt &stmt);
};

} // namespace arsenic

#endif
