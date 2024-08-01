#ifndef __INTERPRETER__
#define __INTERPRETER__

#include <Environment.h>
#include <Lex/Lex.h>
#include <Parse/Expr/AssignExpr.h>
#include <Parse/Expr/BinaryExpr.h>
#include <Parse/Expr/CallExpr.h>
#include <Parse/Expr/Expr.h>
#include <Parse/Expr/ExprVisitor.h>
#include <Parse/Expr/GroupingExpr.h>
#include <Parse/Expr/LiteralExpr.h>
#include <Parse/Expr/LogicalExpr.h>
#include <Parse/Expr/UnaryExpr.h>
#include <Parse/Expr/VarExpr.h>
#include <Parse/Expr/DyadExpr.h>
#include <Parse/Stmt/BlockStmt.h>
#include <Parse/Stmt/ExpressionStmt.h>
#include <Parse/Stmt/FunctionStmt.h>
#include <Parse/Stmt/IfStmt.h>
#include <Parse/Stmt/PrintStmt.h>
#include <Parse/Stmt/ReturnStmt.h>
#include <Parse/Stmt/StmtVisitor.h>
#include <Parse/Stmt/VarStmt.h>
#include <Parse/Stmt/WhileStmt.h>
#include <RoutineInterface/ArsenicFunction.h>
#include <any>
#include <memory>
#include <unordered_map>

namespace arsenic {

class Interpreter : public ExprVisitor<std::any>, public StmtVisitor<std::any> {

  std::shared_ptr<Environment> globals = std::make_shared<Environment>();
  std::shared_ptr<Environment> environment = globals;
  std::unordered_map<Dyad_expr, int, Dyad_expr_hash, Dyad_expr_equal> Locals;

public:
  void checkNumberOperand(Token operator_t, const std::any &operand);
  void checkNumberOperands(Token operator_t, const std::any &left,
                           const std::any &right);
  bool isEqual(const std::any &a, const std::any &b);

  void interpret(std::vector<std::shared_ptr<Stmt>> &statements);

  void execute(Stmt &stmt);

  void executeBlock(std::vector<std::shared_ptr<Stmt>> &statements,
                    std::shared_ptr<Environment> env);

  void resolve(Dyad_expr expr, int DepthLevel);

  std::any LookUpVariable(Token Name, const Dyad_expr expr);

  std::shared_ptr<Environment> getGlobalEnvironment();

  std::any evaluate(Expr &expr);
  std::any isTruthy(const std::any &object);

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

#endif // !__INTERPRETER__
