#ifndef GRAMMAR_GENERATOR_H
#define GRAMMAR_GENERATOR_H

#include "token.h"
#include "graph.h"
#include "grammar_checker.h"

class GrammarGenerator {
    using vertex_t = Graph::vertex_t;
public:
    class generated {
    public:
        generated(vertex_t v1, vertex_t v2, TokenType n)
            : v1_(v1)
            , v2_(v2)
            , n_(n)
        {}

        vertex_t v1() const {
            return v1_;
        }

        vertex_t v2() const {
            return v2_;
        }

        TokenType n() const {
            return n_;
        }

        struct Comp {
            bool operator()(const generated &lhs, const generated &rhs) {
                if (lhs.v1_ != rhs.v1_) {
                    return lhs.v1_ < rhs.v1_;
                }
                if (lhs.v2_ != rhs.v2_) {
                    return lhs.v2_ < rhs.v2_;
                }
                return lhs.n_->str() < rhs.n_->str();
            }
        };

        friend std::ostream &operator<<(std::ostream &os, const generated &rhs) {
            return os << "(" << rhs.v1() << ", " << rhs.v2() << ", " << *rhs.n() << ")\n";
        }
    private:
        vertex_t v1_, v2_;
        TokenType n_;
    };
    using generated_container = std::vector<generated>;

    GrammarGenerator(const GrammarChecker &gc, const Graph gr);

    generated_container floyd(bool verbose=false);

private:
    vvst d_;
    const GrammarChecker & gc_;
    const Graph &gr_;
    std::map<ptt, st> pair_rules_reversed_;
};

#endif // GRAMMAR_GENERATOR_H
