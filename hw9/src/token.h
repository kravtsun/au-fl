#ifndef TOKEN_H
#define TOKEN_H

#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <memory>
#include <set>
#include <cassert>

#include "common.h"

#define ALTERNATIVE_SEPARATOR "|"
#define EPSILON "eps"

/** friend TokenFactory.
 *
 */

struct TokenFactory;

struct Token {
    Token(const Token &rhs) = delete;

    static bool isEpsilon(const std::string &s);
    bool isEpsilon() const;

    static bool isTerminal(const std::string &s);
    bool isTerminal() const;

    static bool isNonTerminal(const std::string &s);
    bool isNonTerminal() const;

    const std::string &str() const;

    virtual std::string type() const;

    friend std::ostream &operator<<(std::ostream &os, const Token &token);

    operator bool() const;

protected:
    std::string s_;

    Token(const std::string s) : s_(s) {}

    friend TokenFactory;
};

using TokenType = std::shared_ptr<Token>;

struct Terminal : public Token {
    std::string type() const override;

protected:
    Terminal(const std::string &s) : Token(s) {}
    friend TokenFactory;
};

struct NonTerminal : public Token {
    std::string type() const override;

    friend std::istream& operator>>(std::istream &is, NonTerminal &rhs);

protected:
    NonTerminal(const std::string &s) : Token(s) {}
    friend TokenFactory;
};

struct Epsilon : public Token {
    Epsilon();
    std::string type() const override;
};

struct TokenFactory {
    struct TokenTypeComp {
        bool operator()(const TokenType &lhs, const TokenType &rhs) const;
    };

    struct TokenTypePairComp {
        bool operator()(const std::pair<TokenType, TokenType> &lhs, const std::pair<TokenType, TokenType> &rhs) const;
    };

    static const std::set<TokenType, TokenTypeComp>&all_non_terminals();

    static int non_terminal_index(const TokenType &t);

    static TokenType factory(const std::string &);
private:
    static std::unordered_map<std::string, TokenType> map_;
    static std::set<TokenType, TokenTypeComp> non_terminals;
};


#endif // TOKEN_H

