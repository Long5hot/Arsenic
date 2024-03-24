#ifndef __EXPRVISITOR__
#define __EXPRVISITOR__

namespace arsenic {

class BinaryExpr;
class GroupingExpr;
class LiteralExpr;
class UnaryExpr;

template <typename R> class ExprVisitor {

public:
  virtual ~ExprVisitor() = default;

  virtual R visit(const BinaryExpr &) = 0;
  virtual R visit(const GroupingExpr &) = 0;
  virtual R visit(const LiteralExpr &) = 0;
  virtual R visit(const UnaryExpr &) = 0;
};

} // namespace arsenic

#endif
