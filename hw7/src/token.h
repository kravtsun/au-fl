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

#define forn(i, n) for (int i = 0; i < (int)(n); ++i)
#define all(x) (x).begin(), (x).end()

#define ALTERNATIVE_SEPARATOR "|"
#define EPSILON "eps"

/** friend TokenFactory.
 *
 */

struct TokenFactory;

struct Token {
    Token(const Token &rhs) = delete;

    static bool isEpsilon(const std::string &s);
    bool isEpsilon() const {
        return isEpsilon(s_);
    }

    static bool isTerminal(const std::string &s);
    bool isTerminal() const {
        return isTerminal(s_);
    }

    static bool isNonTerminal(const std::string &s) {
        return !isEpsilon(s) && !isTerminal(s);
    }
    bool isNonTerminal() const {
        return isNonTerminal(s_);
    }

    const std::string &str() const {
        return s_;
    }

    virtual std::string type() const {
        return "Token";
    }

    friend std::ostream &operator<<(std::ostream &os, const Token &token);

    operator bool() const {
        return !s_.empty();
    }

protected:
    std::string s_;

    Token(const std::string s) : s_(s) {}

    friend TokenFactory;
};

using TokenType = std::shared_ptr<Token>;

struct Terminal : public Token {
    std::string type() const override {
        return "Terminal";
    }

protected:
    Terminal(const std::string &s) : Token(s) {}
    friend TokenFactory;
};

struct NonTerminal : public Token {
    std::string type() const override {
        return "Nonterminal";
    }

    friend std::istream& operator>>(std::istream &is, NonTerminal &rhs) {
        return is >> rhs.s_;
    }

protected:
    NonTerminal(const std::string &s) : Token(s) {}
    friend TokenFactory;
};

struct Epsilon : public Token {
    Epsilon() : Token(EPSILON) {}
    std::string type() const override {
        return "eps";
    }
};

struct TokenFactory {
    struct TokenTypeComp {
        bool operator()(const TokenType &lhs, const TokenType &rhs) const {
            return lhs->str() < rhs->str();
        }
    };

    struct TokenTypePairComp {
        bool operator()(const std::pair<TokenType, TokenType> &lhs, const std::pair<TokenType, TokenType> &rhs) const {
            if (lhs.first->str() != rhs.first->str()) {
                return lhs.first->str() < rhs.first->str();
            }
            return lhs.second->str() < rhs.second->str();
        }
    };

    static const auto &all_non_terminals() {
        return non_terminals;
    }

    static int non_terminal_index(const TokenType &t) {
        assert(t->isNonTerminal());
        auto comp = [&t](const TokenType &tt) -> bool { return tt->str() == t->str(); };
        auto it = std::find_if(all(non_terminals), comp);
        return (int)(std::distance(non_terminals.begin(), it));
    }

    static TokenType factory(const std::string &);
private:
    static std::unordered_map<std::string, TokenType> map_;
    static std::set<TokenType, TokenTypeComp> non_terminals;
    // TODO: deal with cases when nonterminals' names start or end with '
};


#endif // TOKEN_H

