#include "token.h"

Rule Rule::empty = Rule(NonTerminal{""}, Rule::alternatives_type({}));

bool Token::isEpsilon(const std::string &s) {
    return s == EPSILON;
}

bool Token::isTerminal(const std::string &s) {
    bool res = s.front() == '\'';
    assert(!res || (s.back() == '\'' && s.size() > 1));
    return res;
}

Token *Token::factory(const std::string &s) {
    if (isEpsilon(s)) {
        return new Epsilon();
    }
    else if (isTerminal(s)) {
        return new Terminal(s);
    }
    else {
        return new NonTerminal(s);
    }
}

std::ostream &operator<<(std::ostream &os, const Token &token) {
    if (auto ptr = dynamic_cast<const Epsilon *>(&token)) {
        (void)ptr;
        os << token.type();
    }
    else {
        os << token.type() << "(" << token.s_ << ")";
    }
    return os;
}
