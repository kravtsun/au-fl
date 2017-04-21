#include <algorithm>
#include <cassert>
#include "grammar.h"
#include "grammar_converter.h"

int ParseTree::node_cnt_;

Grammar::Grammar(std::istream &is) {
    std::getline(is, start_name_);

    GrammarParser gp(is);
    while (gp) {
        Rules rules = gp.parse_line();
        std::copy(all(rules), back_inserter(rules_));
    }
}

void Grammar::print_rules() {
    std::cout << "START: " << start_name_ << std::endl;
    for (auto const &r : rules_) {
        std::cout << *r.left() << " = " << r.right() << std::endl;
    }
}

void Grammar::normalize() {
    GrammarConverter gc{start_name_, rules_};
    gc.convertToChomsky();
    assert(gc.isNormal());
    rules_ = gc.rules();
}

bool Grammar::check(const std::__cxx11::string &word) {
    word_ = word;
    pt_.clear();

    if (word.empty()) {
        auto rule_has_epsilon = [](const Rule &r) -> bool { return r.right().size() == 1 && r.right().front()->isEpsilon(); };
        return std::find_if(all(rules_), rule_has_epsilon) != rules_.end();
    }

    int n = static_cast<int>(word.size());
    int N = (int)TokenFactory::all_non_terminals().size();

    dp_.assign(N, vvbool(n, vbool(n, false)));
    std::vector<Rule> vrules(all(rules_));

    forn(ri, vrules.size()) {
        auto &r = vrules[ri];
        auto &a = r.right();
        assert(!a.empty());
        if (a.size() > 1 || !a.front()->isTerminal()) continue;
        const auto &s = a.front()->str();
        assert(s.size() == 3 && s.front() == '\'' && s.back() == '\'');
        char c = s[1];

        const int nti = TokenFactory::non_terminal_index(r.left());
        assert(nti < N);
        forn(i, word.size()) {
            if (word[i] == c) {
                std::string str(1, word_[i]);
                auto ptnode = new ParseTree::Node(r.left(), str, i, i);
                pt_.create_node(nti, i, i, ptnode);
                dp_[nti][i][i] = true;
            }
        }
    }

    for (int m = 1; m < n; ++m) {
        forn(a, vrules.size()) {
            auto &r = vrules[a];
            auto &v = r.right();
            if (v.size() == 1) {
                assert(v.front()->isTerminal() || v.front()->isEpsilon());
                continue;
            }

            int ia = TokenFactory::non_terminal_index(r.left());
            assert(v.size() == 2);
            auto b = v.front(), c = v.back();
            int ib = TokenFactory::non_terminal_index(b),
                    ic = TokenFactory::non_terminal_index(c);

            for (int i = 0; i + m < n; ++i) {
                const int j = i + m;
                for (int k = i; k <= j-1; ++k) {
                    if (dp_[ib][i][k] && dp_[ic][k+1][j]) {
                        auto str = word_.substr(i, j-i+1);
                        auto ptnode = new ParseTree::Node(r.left(), str, i, j);
                        pt_.create_node(ia, i, j, ptnode);
                        ptnode->left_ = pt_.get_node(ib, i, k);
                        ptnode->right_ = pt_.get_node(ic, k+1, j);
                        dp_[ia][i][j] = true;
                    }
                }
            }
        }
    }
    auto start_token = TokenFactory::factory(start_name_);
    auto istart = TokenFactory::non_terminal_index(start_token);

    auto result = dp_[istart][0][n-1];
    if (result) {
        pt_.root = pt_.get_node(istart, 0, n-1);
    }

    return result;
}

void Grammar::show_table() {
    const auto &nts = TokenFactory::all_non_terminals();
    const int n = (int)word_.size();

#if PRINT_CSV
    std::cout << "label,left,right,substr\n";
#endif
    int nti = 0;
    for (auto it = nts.begin(); it != nts.end(); ++it, ++nti) {
        std::string name = (*it)->str();
#if !PRINT_CSV
        std::cout << name << ":\n";
#endif
        forn(i, n) {
            for (int j = i; j < n; ++j) {
                if (dp_[nti][i][j]) {
                    auto substr = word_.substr(i, j-i+1);
#if PRINT_CSV
                    std::cout << name << "," << i << "," << j << "," << substr << std::endl;
#else
                    std::cout << "word(" << i << "," << j << ") = " << substr << std::endl;
#endif
                }
            }
        }
    }
}
