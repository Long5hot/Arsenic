#include <Error/arsenic_runtime_error>
#include <Parse/Interpret.h>
#include <any>
#include <charconv>
#include <cstddef>
#include <iostream>
#include <memory>
#include <vector>

namespace arsenic {

std::any Interpreter::evaluate(Expr &expr) { return expr.accept(*this); }

void Interpreter::checkNumberOperand(Token operator_t,
                                     const std::any &operand) {
  if (operand.type() == typeid(double))
    throw new RuntimeError(operator_t, "Operand must be a number.");
}

void Interpreter::checkNumberOperands(Token operator_t, const std::any &left,
                                      const std::any &right) {
  if (left.type() == typeid(double) && right.type() == typeid(double))
    return;
  throw new RuntimeError(operator_t, "Operands must be a numbers.");
}

std::any Interpreter::isTruthy(const std::any &object) {

  if (!object.has_value())
    return false;

  if (object.type() == typeid(bool))
    return std::any_cast<bool>(object);

  return true;
}

bool Interpreter::isEqual(const std::any &a, const std::any &b) {
  if ((!a.has_value()) && (!b.has_value()))
    return true;
  if (!a.has_value())
    return false;

  if (a.type() != b.type())
    return false;
  if (a.type() == typeid(double))
    return std::any_cast<double>(a) == std::any_cast<double>(b);
  if (a.type() == typeid(bool))
    return std::any_cast<bool>(a) == std::any_cast<bool>(b);
  if (a.type() == typeid(std::string))
    return std::any_cast<std::string>(a) == std::any_cast<std::string>(b);
}

std::string stringify(std::any &object) {
  if (!object.has_value())
    return "nil";

  if (object.type() == typeid(double)) {
    return std::to_string(std::any_cast<double>(object));
  }

  if (object.type() == typeid(bool)) {
    if (std::any_cast<bool>(object))
      return "true";
    return "false";
  }

  if (object.type() == typeid(std::string)) {
    return std::any_cast<std::string>(object);
  }

  return "";
}

std::any Interpreter::visit(LiteralExpr &expr) {
  return expr.getLiteralValue();
}

std::any Interpreter::visit(GroupingExpr &expr) {
  return evaluate(expr.getExpression());
}

std::any Interpreter::visit(VarExpr &expr) {
  return environment->get(expr.getToken());
}

std::any Interpreter::visit(LogicalExpr &expr) {
  std::any left = evaluate(expr.getLeftExpr());

  if (expr.getOpToken().getType() == TokenType::OR) {
    if (std::any_cast<bool>(isTruthy(left)))
      return left;
  } else {
    if (!std::any_cast<bool>(isTruthy(left)))
      return left;
  }
  return evaluate(expr.getRightExpr());
}

std::any Interpreter::visit(VarStmt &stmt) {

  std::any objectValue = {};
  if (stmt.hasInitializer())
    objectValue = evaluate(stmt.getInitializer());

  environment->define(stmt.getToken().getLexeme(), objectValue);
  return {};
}

std::any Interpreter::visit(WhileStmt &stmt) {

  while (std::any_cast<bool>(isTruthy(evaluate(stmt.getCondition())))) {
    execute(stmt.getWhileBody());
  }

  return {};
}

std::any Interpreter::visit(AssignExpr &expr) {
  std::any value = evaluate(expr.getValue());
  environment->assign(expr.getToken(), value);
  return value;
}

std::any Interpreter::visit(UnaryExpr &expr) {

  std::any rightNode = evaluate(expr.getExpression());

  switch (expr.getOpLexeme().getType()) {
  case TokenType::MINUS: {
    checkNumberOperand(expr.getOpLexeme(), rightNode);
    return -std::any_cast<double>(rightNode);
  }
  case TokenType::BANG:
    return !std::any_cast<bool>(isTruthy(rightNode));
  }

  return nullptr;
}

std::any Interpreter::visit(BinaryExpr &expr) {

  std::any left = evaluate(expr.getLeftExpr());
  std::any right = evaluate(expr.getRightExpr());

  switch (expr.getOpToken().getType()) {
  case TokenType::PLUS: {
    if ((left.type() == typeid(double)) && (right.type() == typeid(double)))
      return std::any_cast<double>(left) + std::any_cast<double>(right);

    if ((left.type() == typeid(std::string)) &&
        (right.type() == typeid(std::string)))
      return std::any_cast<double>(left) + std::any_cast<double>(right);

    throw new RuntimeError(expr.getOpToken(),
                           "Operands must be two numbers or two strings.");
  }
  case TokenType::GREATER:
    checkNumberOperands(expr.getOpToken(), left, right);
    return std::any_cast<double>(left) > std::any_cast<double>(right);
  case TokenType::GREATER_EQUAL:
    checkNumberOperands(expr.getOpToken(), left, right);
    return std::any_cast<double>(left) >= std::any_cast<double>(right);
  case TokenType::LESS:
    checkNumberOperands(expr.getOpToken(), left, right);
    return std::any_cast<double>(left) < std::any_cast<double>(right);
  case TokenType::LESS_EQUAL:
    checkNumberOperands(expr.getOpToken(), left, right);
    return std::any_cast<double>(left) <= std::any_cast<double>(right);
  case TokenType::BANG_EQUAL:
    return !isEqual(left, right);
  case TokenType::EQUAL_EQUAL:
    return isEqual(left, right);
  case TokenType::MINUS:
    checkNumberOperands(expr.getOpToken(), left, right);
    return std::any_cast<double>(left) - std::any_cast<double>(right);
  case TokenType::SLASH:
    checkNumberOperands(expr.getOpToken(), left, right);
    return std::any_cast<double>(left) / std::any_cast<double>(right);
  case TokenType::STAR:
    checkNumberOperands(expr.getOpToken(), left, right);
    return std::any_cast<double>(left) * std::any_cast<double>(right);
  }
}

std::any Interpreter::visit(ExpressionStmt &stmt) {

  evaluate(stmt.getExpr());
  return {};
}

std::any Interpreter::visit(PrintStmt &stmt) {

  std::any value = evaluate(stmt.getExpr());
  std::cout << stringify(value) << std::endl;
  return {};
}

std::any Interpreter::visit(BlockStmt &stmt) {

  executeBlock(stmt.getStatements(), new Environment(environment));
  return {};
}

void Interpreter::executeBlock(std::vector<std::shared_ptr<Stmt>> &statements,
                               Environment *env) {

  Environment *prevEnvironment = this->environment;

  //  try {
  this->environment = env;

  for (auto &statement : statements)
    execute(*statement);
  //  }
  this->environment = prevEnvironment;
  delete env;
}

std::any Interpreter::visit(IfStmt &stmt) {

  if (std::any_cast<bool>(isTruthy(evaluate(stmt.getCondition()))))
    execute(stmt.getThenBranch());
  else if (stmt.hasElseBranch())
    execute(stmt.getElseBranch());

  return {};
}

// void Interpreter::interpret(std::shared_ptr<Expr> &expression) {
//   try {
//     std::any value = evaluate(expression);
//     std::cout << stringify(value) << std::endl;
//   } catch (RuntimeError* error) {
//     runtimeError(*error);
//   }
// }

void Interpreter::interpret(std::vector<std::shared_ptr<Stmt>> &statements) {
  try {
    for (std::shared_ptr<Stmt> &statement : statements) {
      execute(*statement);
    }
  } catch (RuntimeError *error) {
    runtimeError(*error);
  }
}

void Interpreter::execute(Stmt &stmt) { stmt.accept(*this); }

Interpreter::Interpreter() : environment(new Environment()) {}

Interpreter::~Interpreter() { delete environment; }

} // namespace arsenic
