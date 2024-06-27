#ifndef __ENVIRONMENT__
#define __ENVIRONMENT__

#include <unordered_map>
#include <string>
#include <any>

#include <Lex/Lex.h>

namespace arsenic {

class Environment {

  std::unordered_map<std::string, std::any> values;
  
public:

  void define(std::string name, std::any value);
  std::any get(Token name);
  void assign(Token name, std::any value);
};

}

#endif // !__ENVIRONMENT__
