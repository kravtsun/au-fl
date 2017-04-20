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

    Rule()
        : left_(nullptr)
        , alternative_()
    {}

    // TODO move optimization.
    Rule(TokenType left, const Alternative &alternative)
        : left_(left)
        , alternative_(alternative)
    {
        assert(left == nullptr || left->isNonTerminal());
    }

    bool operator<(const Rule &rhs) const {
        return *left_ < *rhs.left_;
    }

    bool operator==(const Rule &rhs) const {
        return *left_ == *rhs.left_ && alternative_ == rhs.alternative_;
    }

    bool operator!=(const Rule &rhs) const {
        return !(*this == rhs);
    }

    operator bool() const {
        return left_ != nullptr && !alternative_.empty();
    }

    TokenType left() const {
        return left_;
    }

    const Alternative &right() const {
        return alternative_;
    }

//    bool isEpsilonRule() const {
//        return std::all_of(all(alternative_), [](const TokenType &t) -> bool { return t->isEpsilon(); });
//    }

private:
    TokenType left_;
    Alternative alternative_;
};

// should be with eraseable by iterators elements.
using Rules = std::vector<Rule>;

#endif // RULES_H
