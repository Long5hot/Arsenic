#include <Parse/Expr/ExprVisitor.h>

namespace arsenic {

class Interpreter : public ExprVisitor<std::any> {
  
std::any visit(LiteralExpr & expr) {
  return expr.getLiteralValue(); 
}

std::any visit(GroupingExpr & expr) {
    return expr.getExpression();
}
}

}
