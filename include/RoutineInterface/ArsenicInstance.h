#ifndef __ARSENICINSTANCE__
#define __ARSENICINSTANCE__

#include <Error/arsenic_runtime_error>
#include <Lex/Lex.h>
#include <RoutineInterface/ArsenicClass.h>
#include <any>
#include <unordered_map>

namespace arsenic {

class ArsenicInstance {

  ArsenicClass A_Class;
  std::unordered_map<std::string, std::any> Fields;

public:
  ArsenicInstance(ArsenicClass a_class) : A_Class(a_class) {}

  std::string toString() { return A_Class.toString() + " instace"; }

  std::any get(Token Name) {
    if (auto search = Fields.find(Name.getLexeme()); search != Fields.end())
      return search->second;

    throw new RuntimeError(Name,
                           "Undefined property '" + Name.getLexeme() + "'.");
  }
};

} // namespace arsenic

#endif // !__ARSENICINSTANCE__
