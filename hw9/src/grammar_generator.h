#ifndef GRAMMAR_GENERATOR_H
#define GRAMMAR_GENERATOR_H

// could gain some speedup
// if we know number of tokens beforehand
#define MAX_TOKENS 0

#if MAX_TOKENS
#include <bitset>
#endif

#include "token.h"
#include "graph.h"
#include "grammar_checker.h"

class GrammarGenerator {
public:
    using vertex_t = Graph::vertex_t;
#if MAX_TOKENS
    using bitset_t = std::bitset<MAX_TOKENS>;
    using st = bitset_t;
    using vst = std::vector<st>;
    using vvst = std::vector<vst>;
    using ptt = int;
#else
    using st = ::st;
    using vst = ::vst;
    using vvst = ::vvst;
#endif
    class generated {
    public:
        generated(vertex_t v1, vertex_t v2, TokenType n);

        vertex_t v1() const;

        vertex_t v2() const;

        TokenType n() const;

        struct Comp {
            bool operator()(const generated &lhs, const generated &rhs);
        };

        friend std::ostream &operator<<(std::ostream &os, const generated &rhs);
    private:
        vertex_t v1_, v2_;
        TokenType n_;
    };
    using generated_container = std::vector<generated>;

    GrammarGenerator(const GrammarChecker &gc, const Graph gr);

    generated_container floyd(bool verbose=false);

private:
    vvst d_;
    std::map<ptt, st> pair_rules_reversed_;
    const GrammarChecker & gc_;
    const Graph &gr_;
};

#endif // GRAMMAR_GENERATOR_H
