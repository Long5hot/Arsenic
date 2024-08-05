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

    std::any Method = A_Class.findMethod(Name.getLexeme());
    if (Method.type() == typeid(std::shared_ptr<ArsenicFunction>))
      return Method;

    throw new RuntimeError(Name,
                           "Undefined property '" + Name.getLexeme() + "'.");
  }

  void set(Token Name, std::any Value) {
    Fields.insert({Name.getLexeme(), Value});
  }
};

} // namespace arsenic

#endif // !__ARSENICINSTANCE__
