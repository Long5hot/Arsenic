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
class ClassStmt;

template <typename R> class StmtVisitor {

public:
  virtual ~StmtVisitor() = default;

  virtual std::any visit(ExpressionStmt &) = 0;
  virtual std::any visit(PrintStmt &) = 0;
  virtual std::any visit(VarStmt &) = 0;
  virtual std::any visit(BlockStmt &) = 0;
  virtual std::any visit(IfStmt &) = 0;
  virtual std::any visit(WhileStmt &) = 0;
  virtual std::any visit(FunctionStmt &) = 0;
  virtual std::any visit(ReturnStmt &) = 0;
  virtual std::any visit(ClassStmt &) = 0;
};

} // namespace arsenic

#endif
