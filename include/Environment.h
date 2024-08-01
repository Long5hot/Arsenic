#ifndef __ENVIRONMENT__
#define __ENVIRONMENT__

#include <any>
#include <memory>
#include <string>
#include <unordered_map>
#include <Lex/Lex.h>

namespace arsenic {

class Environment : public std::enable_shared_from_this<Environment> {

  std::unordered_map<std::string, std::any> values;
  std::shared_ptr<Environment> enclosing;

public:
  Environment();
  Environment(std::shared_ptr<Environment> enclosing);

  void assignAt(int Distance, Token Name, std::any Value);
  void define(std::string name, std::any value);
  void assign(Token name, std::any value);
  std::shared_ptr<Environment> ancestor(int Distance);
  std::any getAt(int Distance, std::string Name);
  std::any get(Token name);
};

} // namespace arsenic

#endif // !__ENVIRONMENT__
