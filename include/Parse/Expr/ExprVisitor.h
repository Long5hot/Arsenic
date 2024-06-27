#ifndef __EXPRVISITOR__
#define __EXPRVISITOR__

namespace arsenic {

class BinaryExpr;
class GroupingExpr;
class LiteralExpr;
class UnaryExpr;
class VarExpr;
class AssignExpr;

template <typename R> class ExprVisitor {

public:
  virtual ~ExprVisitor() = default;

  virtual R visit(BinaryExpr &) = 0;
  virtual R visit(GroupingExpr &) = 0;
  virtual R visit(LiteralExpr &) = 0;
  virtual R visit(UnaryExpr &) = 0;
  virtual R visit(VarExpr &) = 0;
  virtual R visit(AssignExpr &) = 0;
};

} // namespace arsenic

#endif
