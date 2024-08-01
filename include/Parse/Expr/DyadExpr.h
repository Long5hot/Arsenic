#ifndef __DYADEXPR__
#define __DYADEXPR__

#include <boost/functional/hash.hpp>
#include <Parse/Expr/AssignExpr>
#include <Parse/Expr/VarExpr>
#include <variant>

using Dyad_expr = std::variant<VarExpr, AssignExpr>;

struct Dyad_expr_hash {
    std::size_t operator()(const Dyad_expr& expr) const {
        return std::visit([](const auto& arg) -> std::size_t {
            std::size_t seed = 0;
            boost::hash_combine(seed, arg.getToken().getType());
            boost::hash_combine(seed, arg.getToken().getLiteralValue());
            return seed;
        }, expr);
    }
};

// Custom equality operator for Dyad_expr
struct Dyad_expr_equal {
    bool operator()(const Dyad_expr& lhs, const Dyad_expr& rhs) const {
        if (lhs.index() != rhs.index()) return false;
        return std::visit([](const auto& arg1, const auto& arg2) -> bool {
            return arg1 == arg2;
        }, lhs, rhs);
    }
};


#endif // !__DYADEXPR__
