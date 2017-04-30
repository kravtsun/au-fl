#include "grammar_generator.h"

GrammarGenerator::GrammarGenerator(const GrammarChecker &gc, const Graph gr)
    : gc_(gc)
    , gr_(gr)
{
    const int n = gr.size();
    d_.assign(n, vst(n));
    forn(from, n) {
        for (auto const &p : gr_.edges().at(from)) {
            vertex_t to = p.first;
            char c = p.second;
            assert(from >= 0 && to >= 0);
            auto &from_to_tokens = d_.at(from).at(to);
#if MAX_TOKENS
            for (auto const &itoken : gc_.symbol_rules_.at(c)) {
                from_to_tokens.set(itoken);
            }
#else
            std::copy(all(gc_.symbol_rules_.at(c)),
                      std::inserter(from_to_tokens, from_to_tokens.end()));
#endif

        }
    }

    for (auto const &r : gc_.pair_rules_) {
        token_t ileft = r.first;
        for (auto const &p : r.second) {
#if MAX_TOKENS
            pair_rules_reversed_[p.first * MAX_TOKENS + p.second].set(ileft);
#else
            pair_rules_reversed_[p].insert(ileft);
#endif
        }
    }
}

GrammarGenerator::generated_container GrammarGenerator::floyd(bool verbose) {
    const int n = gr_.size();
#if MAX_TOKENS
    auto step = [this](const bitset_t &l, const bitset_t &r, bitset_t &res) -> void {
        if (l.none() || r.none()) return;
        forn(i, MAX_TOKENS) {
            if (l.test(i)) {
                ptt p = i * MAX_TOKENS;
                forn(j, MAX_TOKENS) {
                    if (r.test(j)) {
                        auto it = pair_rules_reversed_.find(p + j);
                        if (it == pair_rules_reversed_.end()) {
                            continue;
                        }
                        res |= it->second;
                    }
                }
            }
        }
    };
#else
    auto step = [this](const st&l, const st &r, st &res) -> void {
        ptt p;
        forit(lit, l) {
            p.first = *lit;
            forit(rit, r) {
                p.second = *rit;
                auto it = pair_rules_reversed_.find(p);
                if (it == pair_rules_reversed_.end()) {
                    continue;
                }
                std::copy(all(it->second), std::inserter(res, res.end()));
            }
        }
    };
#endif

    int it = 0;
    bool fl = true;
    while (fl) {
        if (verbose) {
            std::cerr << "it = " << std::endl;
        }
        it++;
        fl = false;
        forn(k, n) {
            forn(i, n) {
                auto const& lst = d_[i][k];
#if MAX_TOKENS
                if (lst.none()) continue;
#else
                if (lst.empty()) continue;
#endif
                forn(j, n) {
                    auto const &rst = d_[k][j];
                    st old = d_[i][j];
                    step(lst, rst, d_[i][j]);
                    if (old != d_[i][j]) {
                        fl = true;
                    }
                }
            }
        }
    }

    generated_container res;
    forn(i, n) {
        forn(j, n) {
            auto &dij = d_.at(i).at(j);
#if MAX_TOKENS
            forn(k, MAX_TOKENS) {
                if (dij.test(k)) {
                    res.emplace_back(i, j, gc_.non_terminals_.at(k));
                }
            }
#else
            auto f = [i,j,this](const token_t &t) -> generated {
                return generated(i, j, gc_.non_terminals_.at(t));
            };
            std::transform(all(dij), std::back_inserter(res), f);
#endif
        }
    }
    return res;
}
