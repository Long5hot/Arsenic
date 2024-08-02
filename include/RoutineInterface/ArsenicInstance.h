#ifndef __ARSENICINSTANCE__
#define __ARSENICINSTANCE__

#include <RoutineInterface/ArsenicClass.h>

namespace arsenic {

class ArsenicInstance {

  ArsenicClass A_Class;

public:
  ArsenicInstance(ArsenicClass a_class) : A_Class(a_class) {}

  std::string toString() { return A_Class.toString() + " instace"; }
};

} // namespace arsenic

#endif // !__ARSENICINSTANCE__
