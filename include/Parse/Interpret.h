#include <Parse/Expr/Expr.h>
#include <Parse/Expr/GroupingExpr.h>
#include <Parse/Expr/BinaryExpr.h>
#include <Parse/Expr/UnaryExpr.h>
#include <Parse/Expr/LiteralExpr.h>
#include <Parse/Expr/ExprVisitor.h>
#include <any>
#include <memory>
#include <Lex/Lex.h>

namespace arsenic {

class Interpreter : public ExprVisitor<std::any> {

public:
std::any visit(LiteralExpr & expr);

std::any visit(GroupingExpr & expr);

std::any visit(UnaryExpr & expr);

std::any visit(BinaryExpr & expr);

std::any evaluate(std::unique_ptr<Expr>& expr);

void checkNumberOperand(Token operator_t, const std::any& operand);

void checkNumberOperands(Token operator_t, const std::any& left, const std::any& right);

std::any isTruthy(const std::any& object);

bool isEqual(const std::any &a, const std::any &b);

void interpret(std::unique_ptr<Expr> &expr);

};

}
