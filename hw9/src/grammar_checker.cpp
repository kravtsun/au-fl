#include <algorithm>
#include "grammar_checker.h"

GrammarChecker::GrammarChecker(const Grammar &gr)
    : has_epsilon_(false)
{
    auto is_epsilon_rule = [](const Rule &r) -> bool {
        return r.right().size() == 1 && r.right().front()->isEpsilon(); };
    auto const &ant = TokenFactory::all_non_terminals();
    int it = 0;
    start_token_ = TokenFactory::factory(gr.start_name());
    assert(start_token_->isNonTerminal());
    for (auto const &t : ant) {
        token_map_[t] = it++;
        assert(token_map_.at(t) == (int)non_terminals_.size());
        non_terminals_.push_back(t);
    }
    for (auto const r : gr.rules()) {
        auto const &a = r.right();
        int ileft = non_terminal_index(r.left());
        if (a.size() > 1) {
            assert(a.size() == 2 &&
                   a.front()->isNonTerminal() &&
                   a.back()->isNonTerminal());
            const int ifirst = non_terminal_index(a.front());
            const int isecond = non_terminal_index(a.back());
            pair_rules_[ileft].emplace_back(ifirst, isecond);
        }
        else if (is_epsilon_rule(r)) {
            has_epsilon_ = true;
        }
        else {
            assert(a.size() == 1 && a.front()->isTerminal());
            auto const &str = a.front()->str();
            assert(str.front() == '\'' && str.back() == '\'' && str.size() == 3);
            symbol_rules_[str[1]].push_back(ileft);
        }
    }

    for (auto &sv : symbol_rules_) {
        std::sort(all(sv.second));
    }

    dp_.resize(non_terminals_.size());
}

ParseTree GrammarChecker::build_tree() {
    if (word_.empty()) {
        return ParseTree(new ParseTree::Node(start_token_, word_, 0, 0));
    }
    ParseTree pt(nullptr);

    forn(i, word_.size()) {
        char c = word_[i];
        forn(itoken, non_terminals_.size()) {
            auto const &t = non_terminals_[itoken];
            if (std::binary_search(all(symbol_rules_[c]), itoken)) {
                std::string str(1, c);
                auto ptnode = new ParseTree::Node(t, str, i, i);
                pt.create_node(itoken, i, i, ptnode);
            }
        }
    }

    const int n = static_cast<int>(word_.size());
    for (int m = 1; m < n; ++m) {
        for(auto const & r : pair_rules_) {
            const int ileft = r.first;
            TokenType left_token = non_terminals_[ileft];
            for (auto const p : r.second) {
                const int ifirst = p.first;
                const int isecond = p.second;
                for (int i = 0, j = m; j < n; ++i, ++j) {
                    for (int k = i; k <= j-1; ++k) {
                        if (dp_[ifirst][i][k] && dp_[isecond][k+1][j]) {
                            std::string str = word_.substr(i, j-i+1);
                            auto ptnode = new ParseTree::Node(left_token, str, i, j);
                            pt.create_node(ileft, i, j, ptnode);
                            ptnode->left_ = pt.get_node(ifirst, i, k);
                            ptnode->right_ = pt.get_node(isecond, k+1, j);
                        }
                    }
                }
            }
        }
    }
    pt.root = pt.get_node(istart(), 0, n-1);
    return pt;
}

bool GrammarChecker::check(const std::string &word) {
    word_ = word;
    if (word.empty()) {
        word_.clear();
        return has_epsilon_;
    }

    int n = static_cast<int>(word.size());
    int N = static_cast<int>(token_map_.size());
    dp_.assign(N, vvbool(n, vbool(n, false)));

    for (auto const &p : symbol_rules_) {
        char ctoken = p.first;
        for (auto const &itoken : symbol_rules_[ctoken]) {
            assert(itoken < N);
            forn(i, word_.size()) {
                if (word_[i] == ctoken) {
                    dp_[itoken][i][i] = true;
                }
            }
        }
    }

    for (int m = 1; m < n; ++m) {
        for(auto const & r : pair_rules_) {
            const int ileft = r.first;
            for (auto const p : r.second) {
                const int ifirst = p.first;
                const int isecond = p.second;
                for (int i = 0, j = m; j < n; ++i, ++j) {
                    for (int k = i; k <= j-1; ++k) {
                        const bool b = dp_[ifirst][i][k] && dp_[isecond][k+1][j];
                        dp_[ileft][i][j] = dp_[ileft][i][j] || b;
                    }
                }
            }
        }
    }
    return dp_[istart()][0][n-1];
}

void GrammarChecker::show_table(bool print_csv) {
    const int n = (int)word_.size();

    if (print_csv) {
        std::cout << "label,left,right,substr\n";
    }
    forn(nti, non_terminals_.size()) {
        std::string name = non_terminals_[nti]->str();
        if (!print_csv) {
            std::cout << name << ":\n";
        }
        forn(i, n) {
            for (int j = i; j < n; ++j) {
                if (dp_[nti][i][j]) {
                    auto substr = word_.substr(i, j-i+1);
                    if (print_csv) {
                        std::cout << name << "," << i << "," << j << "," << substr << std::endl;
                    }
                    else {
                        std::cout << "word(" << i << "," << j << ") = " << substr << std::endl;
                    }
                }
            }
        }
    }
}

int GrammarChecker::istart() const {
    return token_map_.at(start_token_);
}

int GrammarChecker::non_terminal_index(const TokenType &t) {
    assert(t->isNonTerminal());
    return token_map_.at(t);
}
