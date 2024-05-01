#include <Parse/Expr/LiteralExpr.h>

namespace arsenic {

LiteralExpr::LiteralExpr(std::any value_t) : literal_value(value_t) {}

std::any
LiteralExpr::accept(const ExprVisitor<std::any> &visitor) const {
  return visitor.visit(*this);
}

std::any LiteralExpr::getLiteralValue() const {
  return literal_value;
} 

std::string LiteralExpr::getLiteralStringValue() const { 

  // This is Wrong Do not use this...
    
//  return literal_value;
      try {
        if (literal_value.type() == typeid(std::string)) {
            return std::any_cast<std::string>(literal_value);
        } else if (literal_value.type() == typeid(int)) {
            return std::to_string(std::any_cast<int>(literal_value));
        } else if (literal_value.type() == typeid(double)) {
            return std::to_string(std::any_cast<double>(literal_value));
        }
        // Add more types as needed
        else {
            return "Unsupported type";
        }
    } catch (const std::bad_any_cast& e) {
        return "Conversion failed: " + std::string(e.what());
    }


}

} // namespace arsenic
