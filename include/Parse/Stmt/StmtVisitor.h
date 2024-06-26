#ifndef __STMTVISITOR__
#define __STMTVISITOR__

namespace arsenic {

class ExpressionStmt;
class PrintStmt;
class VarStmt;
class BlockStmt;

template <typename R> class StmtVisitor {

public:
  virtual ~StmtVisitor() = default;

  virtual std::any visit(ExpressionStmt &) = 0;
  virtual std::any visit(PrintStmt &) = 0;
  virtual std::any visit(VarStmt &) = 0;
  virtual std::any visit(BlockStmt &) = 0;
};

} // namespace arsenic

#endif
