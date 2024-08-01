#ifndef __EXPRVISITOR__
#define __EXPRVISITOR__

#include <variant>

namespace arsenic {

class BinaryExpr;
class GroupingExpr;
class LiteralExpr;
class UnaryExpr;
class VarExpr;
class AssignExpr;
class LogicalExpr;
class CallExpr;

using Dyad_expr = std::variant<VarExpr, AssignExpr>;

template <typename R> class ExprVisitor {

public:
  virtual ~ExprVisitor() = default;

  virtual R visit(BinaryExpr &) = 0;
  virtual R visit(GroupingExpr &) = 0;
  virtual R visit(LiteralExpr &) = 0;
  virtual R visit(UnaryExpr &) = 0;
  virtual R visit(VarExpr &) = 0;
  virtual R visit(AssignExpr &) = 0;
  virtual R visit(LogicalExpr &) = 0;
  virtual R visit(CallExpr &) = 0;
};

} // namespace arsenic

#endif
