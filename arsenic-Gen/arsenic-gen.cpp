#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <utility>

//  Thought was to store data in the form of
//  std::list<std::pair<std::string,
//  std::list<std::pair<std::string,std::string>>>> i.e. Remove generating table
//  twice. But it's not easy to understand and need to use auto every time to
//  create iterator. Not easy to debug as well. Reason being, generating table
//  twice.

// Currently Data-Structure is std::list<std::pair<std::string, std::string>>
// Which holds data in the manner of
//            "Binary : Expr left, Token oprtr, Expr  right"
//            "Grouping : Expr expression",
//            "Literal : Object value",
//            "Unary : Token oprtr, Expr right"
//
// Again, second time table is generated in the same format like
// std::list<std::pair<std::string, std::string>>
//
// TODO : Thinking of creating new data structure which can hold this format and
// we don't need to parse this table twice.
//
namespace arsenic_gen {

class generateGrammer {

  using methodField = std::pair<std::string, std::string>;
  using fieldTable = std::list<methodField>;
  using typeTable = fieldTable;
  using typeIter = methodField;
  std::string file;
  std::string baseName = "Expr";
  std::string fileContent;
  const char *whitespaces = " \t\f\v\n\r";

  fieldTable generateFieldTable(const std::string &classFields) {

    fieldTable table;
    std::size_t foundComma = classFields.find(',');
    std::size_t lastComma = 0;
    std::size_t foundSpace = classFields.find(' ');
    while (foundComma != std::string::npos) {
      table.push_back(std::make_pair(
          classFields.substr(lastComma, foundSpace - lastComma),
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

  typeTable generateTypeTable(const std::list<std::string> &grammerClasses) {

    typeTable table;

    for (std::string classes : grammerClasses) {
      std::size_t colonPlace = classes.find(':');
      std::string className = classes.substr(0, colonPlace - 1);
      className.erase(className.find_last_not_of(whitespaces) +
                      1); // Remove trailing whitespaces.

      std::string classFields = classes.substr(colonPlace + 1);
      // Remove leading whitespaces.
      classFields.erase(0, classFields.find_first_not_of(whitespaces));
      // Remove trailing whitespaces.
      classFields.erase(classFields.find_last_not_of(whitespaces) + 1);
      // defineType(className, classFields);
      table.push_back(std::make_pair(className, classFields));
    }
    return table;
  }

  void defineVisitor(typeTable &table_t) {

    // Declare prototypes to be used.
    fileContent.append("class " + baseName + ";\n");

    for (const typeIter &type_t : table_t)
      fileContent.append("class " + type_t.first + ";\n");

    fileContent.append("\n\n");
    fileContent.append("class Visitor {\n\n");
    fileContent.append("  virtual ~Visitor() = default;\n\n");

    for (const typeIter &type_t : table_t)
      fileContent.append("  virtual void visit(const " + type_t.first +
                         "& ) = 0;\n");

    fileContent.append("};\n\n");
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
    fileContent.append("{}\n\n");

    fileContent.append("  void accept(const Visitor & visitor) {\n");
    fileContent.append("    visitor.visit(*this);\n  }\n};\n\n");
  }

  void includeHeaders() {
    fileContent.append("#include <variant>\n");
    fileContent.append("#include <string>\n");
    fileContent.append("#include <Lex/lex.h>\n");
    fileContent.append("\n\n");
  }

  void usingDirectives() {
    fileContent.append("using literal_variant = std::variant<std::monostate, "
                       "std::string, double>;\n\n");
    fileContent.append("using namespace arsenic;\n\n");
  }

public:
  void defineGrammer(std::list<std::string> grammerClasses) {

    // Initiate writing contentent into buffer and flush the buffer into disk
    // when finished.
    typeTable grammerTypeTable = generateTypeTable(grammerClasses);

    includeHeaders();
    usingDirectives();

    defineVisitor(grammerTypeTable);

    fileContent.append("class " + baseName + " {\n\n");

    fileContent.append("  virtual void accept(const Visitor &) = 0;\n");
    fileContent.append("};\n\n");

    for (const typeIter &iter : grammerTypeTable)
      defineType(iter.first, iter.second);
  }

  void generateFile() {
    std::ofstream file_t(file + ".cpp");
    file_t << fileContent;
    file_t.close();
  }

  generateGrammer(const std::string &file) : file(file) {}
  generateGrammer() = delete;
};
} // namespace arsenic_gen

int main(int argc, char *argv[]) {

  if (argc < 2) {
    std::cout << "Usage: arsenic-gen <output filename>" << std::endl;
    exit(0);
  }

  arsenic_gen::generateGrammer grm(argv[1]);
  grm.defineGrammer({"Binary   : Expr left, Token oprtr, Expr right",
                     "Grouping : Expr expression",
                     "Literal  : literal_variant literal_value",
                     "Unary    : Token oprtr, Expr expression"});

  grm.generateFile();
}
