#include <Resolver/Resolver.h>
#include <memory>
#include <shared_mutex>
#include <type_traits>
#include <vector>

namespace arsenic {

Resolver::Resolver(std::shared_ptr<Interpreter> interpreter)
    : interpreter(interpreter) {}

void Resolver::BeginScope() {
  Scopes.push_back(std::make_shared<std::unordered_map<std::string, bool>>());
}

void Resolver::EndScope() { Scopes.pop_back(); }

void Resolver::resolve(std::vector<std::shared_ptr<Stmt>> statements) {

  for (std::shared_ptr<Stmt> statement : statements)
    resolve(*statement);
}

void Resolver::resolve(Stmt &stmt) { stmt.accept(*this); }

void Resolver::resolve(Expr &expr) { expr.accept(*this); }

void Resolver::declare(Token Name) {

  if (Scopes.empty())
    return;

  std::shared_ptr<std::unordered_map<std::string, bool>> Scope = Scopes.back();

  if (auto search = Scope->find(Name.getLexeme()); search != Scope->end())
    arsenicError.error(Name,
                       "Already a variable with this name in this scope.");

  Scope->insert({Name.getLexeme(), false});
}

void Resolver::define(Token Name) {
  if (Scopes.empty())
    return;

  Scopes.back()->insert_or_assign(Name.getLexeme(), true);
}

void Resolver::resolveLocal(Dyad_expr expr, Token Name) {
  for (int i = Scopes.size() - 1; i >= 0; --i)
    if (auto Search = Scopes.at(i)->find(Name.getLexeme());
        Search != Scopes.at(i)->end()) {
      interpreter->resolve(expr, Scopes.size() - 1 - i);
    }
}

void Resolver::resolveFunction(FunctionStmt &function, FunctionType Type) {

  FunctionType EnclosingFunction = CurrentFunction;
  CurrentFunction = Type;
  BeginScope();

  for (Token param : function.getParameters()) {
    declare(param);
    define(param);
  }

  resolve(function.getFunctionBody());
  EndScope();
  CurrentFunction = EnclosingFunction;
}

std::any Resolver::visit(BlockStmt &stmt) {

  BeginScope();
  resolve(stmt.getStatements());
  EndScope();
  return {};
}

std::any Resolver::visit(VarStmt &stmt) {
  declare(stmt.getToken());

  if (stmt.hasInitializer())
    resolve(stmt.getInitializer());

  define(stmt.getToken());
  return {};
}

std::any Resolver::visit(VarExpr &expr) {
  if (!Scopes.empty())
    if (auto search = Scopes.back()->find(expr.getToken().getLexeme());
        search != Scopes.back()->end()) {
      if (search->second == false)
        arsenicError.error(expr.getToken(),
                           "Can't read local variable in its own initializer.");
    }
  resolveLocal(expr, expr.getToken());
  return {};
}

std::any Resolver::visit(AssignExpr &expr) {
  resolve(expr.getValue());
  resolveLocal(expr, expr.getToken());
  return {};
}

std::any Resolver::visit(FunctionStmt &stmt) {
  declare(stmt.getToken());
  define(stmt.getToken());

  resolveFunction(stmt, FunctionType::FUNCTION);
  return {};
}

std::any Resolver::visit(ExpressionStmt &stmt) {
  resolve(stmt.getExpr());
  return {};
}

std::any Resolver::visit(IfStmt &stmt) {
  resolve(stmt.getCondition());
  resolve(stmt.getThenBranch());

  if (stmt.hasElseBranch())
    resolve(stmt.getElseBranch());

  return {};
}

std::any Resolver::visit(PrintStmt &stmt) {
  resolve(stmt.getExpr());
  return {};
}

std::any Resolver::visit(ReturnStmt &stmt) {

  if (CurrentFunction == FunctionType::NONE)
    arsenicError.error(stmt.getToken(), "Can't return from top-level code.");

  if (stmt.getExpressionValue() != nullptr)
    resolve(*stmt.getExpressionValue());

  return {};
}

std::any Resolver::visit(WhileStmt &stmt) {
  resolve(stmt.getCondition());
  resolve(stmt.getWhileBody());
  return {};
}

std::any Resolver::visit(BinaryExpr &expr) {
  resolve(expr.getLeftExpr());
  resolve(expr.getRightExpr());
  return {};
}

std::any Resolver::visit(CallExpr &expr) {
  resolve(expr.getCallee());

  for (std::shared_ptr<Expr> argument : expr.getArguments())
    resolve(*argument);

  return {};
}

std::any Resolver::visit(LogicalExpr &expr) {
  resolve(expr.getLeftExpr());
  resolve(expr.getRightExpr());
  return {};
}

std::any Resolver::visit(UnaryExpr &expr) {
  resolve(expr.getExpression());
  return {};
}

std::any Resolver::visit(GroupingExpr &expr) {
  resolve(expr.getExpression());
  return {};
}

std::any Resolver::visit(LiteralExpr &expr) { return {}; }

} // namespace arsenic
