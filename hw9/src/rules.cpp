#include "rules.h"

std::istream &operator>>(std::istream &is, Alternative &rhs) {
    rhs.clear();
    std::string token_str;
    while (static_cast<bool>(is >> token_str) && token_str != ALTERNATIVE_SEPARATOR) {
        rhs.emplace_back(TokenFactory::factory(token_str));
    }
    return is;
}

std::ostream &operator<<(std::ostream &os, const Alternative &rhs) {
    auto f = [&os](const TokenType &ptr) { os << *ptr; };
    for (auto it = rhs.cbegin(); it != rhs.cend(); it++) {
        f(*it);
        if ((it + 1) != rhs.cend()) {
            os << " ";
            //                os << " " ALTERNATIVE_SEPARATOR " ";
        }
    }
    //        std::for_each(rhs.cbegin(), rhs.cend(), f);
    return os;
}

Rule::Rule()
    : left_(nullptr)
    , alternative_()
{}

Rule::Rule(TokenType left, const Alternative &alternative)
    : left_(left)
    , alternative_(alternative)
{
    assert(left == nullptr || left->isNonTerminal());
}

TokenType Rule::left() const {
    return left_;
}

const Alternative &Rule::right() const {
    return alternative_;
}

Rule::operator bool() const {
    return left_ != nullptr && !alternative_.empty();
}
