#include "grammar_generator.h"



GrammarGenerator::GrammarGenerator(const GrammarChecker &gc, const Graph gr)
    : gc_(gc)
    , gr_(gr)
{
    int n = gr.size();
    d_.assign(n, vst(n));

    forn(from, n) {
        for (auto const &p : gr_.edges().at(from)) {
            vertex_t to = p.first;
            char c = p.second;
            assert(from >= 0 && to >= 0);
            st &from_to_tokens = d_.at(from).at(to);
            std::copy(all(gc_.symbol_rules_.at(c)),
                      std::inserter(from_to_tokens, from_to_tokens.end()));
        }
    }

    for (auto const &r : gc_.pair_rules_) {
        token_t ileft = r.first;
        for (auto const &p : r.second) {
            pair_rules_reversed_[p].insert(ileft);
        }
    }
}

GrammarGenerator::generated_container GrammarGenerator::floyd() {
    const int n = gr_.size();
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

    int it = 0;
    bool fl = true;
    while (fl) {
        printf("it = %d\n", it);
        it++;
        fl = false;
        forn(k, n) {
            forn(i, n) {
                auto const& lst = d_[i][k];
                if (lst.empty()) continue;
                forn(j, n) {
                    auto const &rst = d_[k][j];
                    if (rst.empty()) continue;
                    st s = d_[i][j];
                    step(lst, rst, s);
                    if (s != d_[i][j]) {
                        fl = true;
                        d_[i][j] = s;
                    }
                }
            }
        }
    }

    generated_container res;
    forn(i, n) {
        forn(j, n) {
            auto f = [i,j,this](const token_t &t) -> generated {
                return generated(i, j, gc_.non_terminals_.at(t));
            };
            auto &dij = d_.at(i).at(j);
            std::transform(all(dij), std::back_inserter(res), f);
        }
    }
    return res;
}
