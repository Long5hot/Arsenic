#ifndef __STMT__
#define __STMT__

#include <Parse/Stmt/StmtVisitor.h>
#include <any>

namespace arsenic {

class Stmt {

public:
  virtual ~Stmt() = default;

  virtual std::any accept(StmtVisitor<std::any> &) = 0;
};

} // namespace arsenic

#endif
