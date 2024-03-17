#include <iostream>
#include <list>
#include <string>
#include <utility>

namespace arsenic_gen {

class generateGrammer {

  std::string outputDir;
  std::string baseName;
  std::string fileContent;
  const char * whitespaces = " \t\f\v\n\r";
  using methodField = std::pair<std::string, std::string>;
  using fieldTable = std::list<methodField>;

public:


  fieldTable generateFieldTable(const std::string &classFields) {

    fieldTable table;
    std::size_t foundComma = classFields.find(',');
    std::size_t lastComma = 0;
    std::size_t foundSpace = classFields.find(' ');
    while (foundComma != std::string::npos) {
      table.push_back(
          std::make_pair(classFields.substr(lastComma, foundSpace - lastComma),
                         classFields.substr(foundSpace + 1, foundComma - foundSpace - 1)));
      lastComma = foundComma + 1;
      foundComma = classFields.find(',', lastComma);
      foundSpace = classFields.find(' ', lastComma + 1);
    }
    // for last remainig field. It will push the only field as well even if
    // no comma found.
    table.push_back(
        std::make_pair(classFields.substr(lastComma, foundSpace - lastComma),
                       classFields.substr(foundSpace + 1)));
    // table.push_back(classFields.substr(lastComma));
    return table;
  }

  void defineType(const std::string &className,
                  const std::string &classFields) {

    fieldTable fields = generateFieldTable(classFields);

    fileContent.append("class " + className + " : " + "public " + baseName +
                       " {\n\n");

    // Write out fields.
    for (const methodField &field : fields)
      fileContent.append("  " + field.first + " " + field.second + ";\n");

    fileContent.append("public:\n");
    fileContent.append("  " + className + "(" + classFields + ") : ");

    for (const methodField &field : fields)
      fileContent.append(field.second + "(" + field.second + ") ,");
    fileContent.pop_back();
    fileContent.append("{}\n};\n\n\n");
  }

  void defineGrammer(std::list<std::string> grammerClasses) {

    // Initiate writing contentent into buffer and flush the buffer into disk
    // when finished.
    fileContent.append("class " + baseName + " {\n\n");
    fileContent.append("}\n\n");

    for (std::string classes : grammerClasses) {
      std::size_t colonPlace = classes.find(':');
      std::string className = classes.substr(0, colonPlace - 1);
      className.erase(className.find_last_not_of(whitespaces) +
                      1); // Remove trailing whitespaces.

      std::string classFields = classes.substr(colonPlace + 1);
      // Remove leading whitespaces.
      classFields.erase(0, classFields.find_first_not_of(whitespaces));
      // Remove trailing whitespaces.
      classFields.erase(classFields.find_last_not_of(whitespaces)+1);
      defineType(className, classFields);
    }

    std::cout << fileContent << std::endl;
  }

  generateGrammer(const std::string &outputDir, const std::string &baseName)
      : outputDir(outputDir), baseName(baseName) {}
  generateGrammer() = delete;
};
} // namespace arsenic_gen

int main(int argc, char *argv[]) {

  if (argc < 2) {
    std::cout << "Usage: arsenic-gen <output directory>" << std::endl;
    exit(0);
  }

  arsenic_gen::generateGrammer grm(argv[1], "Expr");
  grm.defineGrammer({"Binary   : Expr left, Token operator, Expr right",
                     "Grouping : Expr expression", "Literal  : literal_variant literal_value",
                     "Unary    : Token operator, Expr expression"});
}
