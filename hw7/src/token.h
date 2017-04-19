#ifndef TOKEN_H
#define TOKEN_H

#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <cassert>

#define ALTERNATIVE_SEPARATOR "|"
#define EPSILON "eps"

struct Token {
    Token() = default;

    Token(const std::string s)
        : s_(s)
    {}

    bool operator==(const Token &rhs) const {
        return s_ == rhs.s_;
    }

    static bool isEpsilon(const std::string &s);

    static bool isTerminal(const std::string &s);

    static Token *factory(const std::string &s);

    virtual std::string type() const {
        return "Token";
    }

//    friend std::ostream &operator<<(std::ostream &os, const Token &token) {
//        if (auto ptr = dynamic_cast<Epsilon *>(&token)) {
//            os << token.type();
//        }
//        else {
//            os << token.type() << "(" << token.s_ << ")";
//        }
//        return os;
//    }
    friend std::ostream &operator<<(std::ostream &os, const Token &token);
    std::string s_;
};

struct Terminal : public Token {
    Terminal(const std::string &s) : Token(s) {}

    std::string type() const override {
        return "Terminal";
    }
};

struct NonTerminal : public Token {
    NonTerminal() = default;

    NonTerminal(const std::string &s) : Token(s) {}

    std::string type() const override {
        return "Nonterminal";
    }

    friend std::istream& operator>>(std::istream &is, NonTerminal &rhs) {
        return is >> rhs.s_;
    }
};

struct Epsilon : public Token {
    Epsilon() :
        Token(EPSILON)
    {}
};

struct Alternative : public std::vector<std::shared_ptr<Token>> {
    friend std::istream &operator>>(std::istream &is, Alternative &rhs) {
        std::string token_str;
        while (static_cast<bool>(is >> token_str) && token_str != ALTERNATIVE_SEPARATOR) {
            rhs.emplace_back(Token::factory(token_str));
        }
        return is;
    }

    friend std::ostream &operator<<(std::ostream &os, const Alternative &rhs) {
        auto f = [&os](std::shared_ptr<Token> ptr) { os << *ptr; };
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
};

struct Rule {
    Rule(const Rule &rhs) = default;
    Rule()
        : Rule(empty)
    {}

    Rule(const NonTerminal &left, const std::vector<Alternative> &alternatives)
        : left_(left)
        , alternatives_(alternatives)
    {}

    bool operator==(const Rule &rhs) const {
        return left_ == rhs.left_ && alternatives_ == rhs.alternatives_;
    }

    bool operator!=(const Rule &rhs) const {
        return !(*this == rhs);
    }

    operator bool() const {
        return *this != empty;
    }

    NonTerminal left() const {
        return left_;
    }

    auto alternatives() const {
        return alternatives_;
    }

    static Rule empty;

private:
    NonTerminal left_;
    std::vector<Alternative> alternatives_;
};

#endif // TOKEN_H

