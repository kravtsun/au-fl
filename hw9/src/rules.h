#ifndef RULES_H
#define RULES_H

#include <list>
#include "token.h"

using Alternative = std::vector<TokenType>;

std::istream &operator>>(std::istream &is, Alternative &rhs);

std::ostream &operator<<(std::ostream &os, const Alternative &rhs);

struct Rule {
//    using alternatives_type = std::vector<Alternative>;

    Rule(const Rule &rhs) = default;

    Rule();

    // TODO move optimization.
    Rule(TokenType left, const Alternative &alternative);

    operator bool() const;

    TokenType left() const;

    const Alternative &right() const;

//    bool isEpsilonRule() const {
//        return std::all_of(all(alternative_), [](const TokenType &t) -> bool { return t->isEpsilon(); });
//    }

private:
    TokenType left_;
    Alternative alternative_;
};

// should be with eraseable by iterators elements.
using Rules = std::list<Rule>;

#endif // RULES_H
