#ifndef RULES_H
#define RULES_H

#include "token.h"

using Alternative = std::vector<TokenType>;

std::istream &operator>>(std::istream &is, Alternative &rhs);

std::ostream &operator<<(std::ostream &os, const Alternative &rhs);

//struct Alternative {
//    using vector = std::vector<TokenType>;
//    template<typename ...Args>
//    Alternative(Args &&...args): vector(args...) {}

//    friend std::istream &operator>>(std::istream &is, Alternative &rhs) {
//        rhs.clear();
//        std::string token_str;
//        while (static_cast<bool>(is >> token_str) && token_str != ALTERNATIVE_SEPARATOR) {
//            rhs.emplace_back(TokenFactory::factory(token_str));
//        }
//        return is;
//    }

//    friend std::ostream &operator<<(std::ostream &os, const Alternative &rhs) {
//        auto f = [&os](const TokenType &ptr) { os << *ptr; };
//        for (auto it = rhs.cbegin(); it != rhs.cend(); it++) {
//            f(*it);
//            if ((it + 1) != rhs.cend()) {
//                os << " ";
////                os << " " ALTERNATIVE_SEPARATOR " ";
//            }
//        }
////        std::for_each(rhs.cbegin(), rhs.cend(), f);
//        return os;
//    }
//};

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

    const Token &left() const {
        return *left_;
    }

    const Alternative &right() const {
        return alternative_;
    }

//    const alternatives_type &alternatives() const {
//        return alternatives_;
//    }

//    alternatives_type &alternatives() {
//        return alternatives_;
//    }

private:
    TokenType left_;
    Alternative alternative_;
};

using Rules = std::vector<Rule>;

#endif // RULES_H
