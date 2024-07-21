#ifndef __ENVIRONMENT__
#define __ENVIRONMENT__

#include <any>
#include <memory>
#include <string>
#include <unordered_map>

#include <Lex/Lex.h>

namespace arsenic {

class Environment {

  std::unordered_map<std::string, std::any> values;
  std::shared_ptr<Environment> enclosing;

public:
  Environment();
  Environment(std::shared_ptr<Environment> enclosing);

  void define(std::string name, std::any value);
  std::any get(Token name);
  void assign(Token name, std::any value);
};

} // namespace arsenic

#endif // !__ENVIRONMENT__
