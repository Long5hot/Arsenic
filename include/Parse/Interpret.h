#include <Environment.h>
#include <Lex/Lex.h>
#include <Parse/Expr/AssignExpr.h>
#include <Parse/Expr/BinaryExpr.h>
#include <Parse/Expr/Expr.h>
#include <Parse/Expr/ExprVisitor.h>
#include <Parse/Expr/GroupingExpr.h>
#include <Parse/Expr/LiteralExpr.h>
#include <Parse/Expr/UnaryExpr.h>
#include <Parse/Expr/VarExpr.h>
#include <Parse/Expr/LogicalExpr.h>
#include <Parse/Stmt/BlockStmt.h>
#include <Parse/Stmt/ExpressionStmt.h>
#include <Parse/Stmt/IfStmt.h>
#include <Parse/Stmt/PrintStmt.h>
#include <Parse/Stmt/StmtVisitor.h>
#include <Parse/Stmt/VarStmt.h>
#include <any>
#include <memory>

namespace arsenic {

class Interpreter : public ExprVisitor<std::any>, public StmtVisitor<std::any> {

  Environment *environment;

public:
  std::any visit(LiteralExpr &expr);

  std::any visit(GroupingExpr &expr);

  std::any visit(UnaryExpr &expr);

  std::any visit(BinaryExpr &expr);

  std::any visit(ExpressionStmt &stmt);

  std::any visit(PrintStmt &stmt);

  std::any visit(VarStmt &stmt);

  std::any visit(BlockStmt &stmt);

  std::any visit(IfStmt &stmt);

  std::any visit(VarExpr &stmt);

  std::any visit(AssignExpr &expr);

  std::any visit(LogicalExpr &expr);

  std::any evaluate(Expr &expr);

  void checkNumberOperand(Token operator_t, const std::any &operand);

  void checkNumberOperands(Token operator_t, const std::any &left,
                           const std::any &right);

  std::any isTruthy(const std::any &object);

  bool isEqual(const std::any &a, const std::any &b);

  void interpret(std::vector<std::unique_ptr<Stmt>> &statements);

  void execute(Stmt &stmt);

  void executeBlock(std::vector<std::unique_ptr<Stmt>> &statements,
                    Environment *env);

  Interpreter();

  ~Interpreter();
};

} // namespace arsenic
