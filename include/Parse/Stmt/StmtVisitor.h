#ifndef __STMTVISITOR__
#define __STMTVISITOR__

namespace arsenic {

class ExpressionStmt;
class PrintStmt;
class VarStmt;
class BlockStmt;
class IfStmt;
class WhileStmt;
class FunctionStmt;
class ReturnStmt;

template <typename R> class StmtVisitor {

public:
  virtual ~StmtVisitor() = default;

  virtual R visit(ExpressionStmt &) = 0;
  virtual R visit(PrintStmt &) = 0;
  virtual R visit(VarStmt &) = 0;
  virtual R visit(BlockStmt &) = 0;
  virtual R visit(IfStmt &) = 0;
  virtual R visit(WhileStmt &) = 0;
  virtual R visit(FunctionStmt &) = 0;
  virtual R visit(ReturnStmt &) = 0;
};

} // namespace arsenic

#endif
