#include "token.h"

bool Token::isEpsilon(const std::string &s) {
    return s == EPSILON;
}

bool Token::isEpsilon() const {
    return isEpsilon(s_);
}

bool Token::isTerminal(const std::string &s) {
    bool res = s.front() == '\'';
    assert(!res || (s.back() == '\'' && s.size() > 1));
    return res;
}

bool Token::isTerminal() const {
    return isTerminal(s_);
}

bool Token::isNonTerminal(const std::string &s) {
    return !isEpsilon(s) && !isTerminal(s);
}

bool Token::isNonTerminal() const {
    return isNonTerminal(s_);
}

const std::string &Token::str() const {
    return s_;
}

std::string Token::type() const {
    return "Token";
}

Token::operator bool() const {
    return !s_.empty();
}

std::unordered_map<std::string, TokenType> TokenFactory::map_;
std::set<TokenType, TokenFactory::TokenTypeComp> TokenFactory::non_terminals;

const std::set<TokenType, TokenFactory::TokenTypeComp> &TokenFactory::all_non_terminals() {
    return non_terminals;
}

int TokenFactory::non_terminal_index(const TokenType &t) {
    assert(t->isNonTerminal());
    auto comp = [&t](const TokenType &tt) -> bool { return tt->str() == t->str(); };
    auto it = std::find_if(all(non_terminals), comp);
    return (int)(std::distance(non_terminals.begin(), it));
}

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
//        os << token.type() << "(" << token.s_ << ")";
        os << token.s_;
    }
    return os;
}

std::string Terminal::type() const {
    return "Terminal";
}

std::string NonTerminal::type() const {
    return "Nonterminal";
}

std::istream &operator>>(std::istream &is, NonTerminal &rhs) {
    return is >> rhs.s_;
}

Epsilon::Epsilon() : Token(EPSILON) {}

std::string Epsilon::type() const {
    return "eps";
}

bool TokenFactory::TokenTypeComp::operator()(const TokenType &lhs, const TokenType &rhs) const {
    return lhs->str() < rhs->str();
}

bool TokenFactory::TokenTypePairComp::operator()(const std::pair<TokenType, TokenType> &lhs, const std::pair<TokenType, TokenType> &rhs) const {
    if (lhs.first->str() != rhs.first->str()) {
        return lhs.first->str() < rhs.first->str();
    }
    return lhs.second->str() < rhs.second->str();
}
