#include "token.h"

bool Token::isEpsilon(const std::string &s) {
    return s == EPSILON;
}

bool Token::isTerminal(const std::string &s) {
    bool res = s.front() == '\'';
    assert(!res || (s.back() == '\'' && s.size() > 1));
    return res;
}

std::unordered_map<std::string, TokenType> TokenFactory::map_;
std::set<TokenType, TokenFactory::TokenTypeComp> TokenFactory::non_terminals;

TokenType TokenFactory::factory(const std::string &s) {
    if (s.empty()) {
        return nullptr;
    }

    auto it = map_.find(s);
    if (it != map_.end()) {
        return it->second;
    }

    Token *res = nullptr;
    if (Token::isEpsilon(s)) {
        res = new Epsilon();
    }
    else if (Token::isTerminal(s)) {
        res = new Terminal(s);
    }
    else {
        assert(Token::isNonTerminal(s));
        res = new NonTerminal(s);
        non_terminals.insert(static_cast<TokenType>(res));
    }

    auto shptr = static_cast<TokenType>(res);
    map_[s] = shptr;

    return shptr;
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
