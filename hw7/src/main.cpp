#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include "token.h"
#include "grammar_parser.h"
#include "grammar_converter.h"

using namespace std;

struct Grammar {
    using vbool = std::vector<bool>;
    using vvbool = std::vector<vbool>;
    using vvvbool = std::vector<vvbool>;

public:
    Grammar(std::istream &is) {
        std::getline(is, start_name_);

        GrammarParser gp(is);
        while (gp) {
            Rules rules = gp.parse_line();
            std::copy(all(rules), back_inserter(rules_));
        }
    }

    void print_rules() {
        std::cout << "START: " << start_name_ << std::endl;
        for (auto const &r : rules_) {
            cout << *r.left() << " = " << r.right() << endl;
        }
    }

    void normalize() {
        GrammarConverter gc{start_name_, rules_};
        gc.convertToChomsky();
        assert(gc.isNormal());
        rules_ = gc.rules();
    }

    bool check(const std::string &word) {
        word_ = word;
        if (word.empty()) {
            auto rule_has_epsilon = [](const Rule &r) -> bool { return r.right().size() == 1 && r.right().front()->isEpsilon(); };
            return std::find_if(all(rules_), rule_has_epsilon) != rules_.end();
        }

//        const auto &nt = TokenFactory::all_non_terminals();
        int n = static_cast<int>(word.size());
        int N = (int)TokenFactory::all_non_terminals().size();
//        using TokenTypeComp = TokenFactory::TokenTypeComp;
//        std::map<TokenType, std::vector<Alternative>, TokenTypeComp> nt_rules;
//        for (auto it = rules_.begin(); it != rules_.begin(); ++it) {
//            if (it->right().front()->isEpsilon()) continue;
//            nt_rules[it->left()].push_back(it->right());
//        }

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
//                        std::cout << dp_[ib][i][k] << dp_[ic][k+1][j] << std::endl;
                        if (dp_[ib][i][k] && dp_[ic][k+1][j]) {
                            dp_[ia][i][j] = true;
                        }
                    }
                }
            }
        }
        auto start_token = TokenFactory::factory(start_name_);
        auto istart = TokenFactory::non_terminal_index(start_token);

        return dp_[istart][0][n-1];
    }

    void show_table() {
        const auto &nts = TokenFactory::all_non_terminals();
        //        const int N = (int)nts.size();
        const int n = (int)word_.size();
        const std::string tab = "\t";

        int nti = 0;
        for (auto it = nts.begin(); it != nts.end(); ++it, ++nti) {
            std::cout << (*it)->str() << ":\n";
            forn(i, n) {
                for (int j = i; j < n; ++j) {
                    if (dp_[nti][i][j]) {
                        std::cout << "word(" << i << "," << j << ") = " << word_.substr(i, j-i+1) << std::endl;
                    }
                }
            }
//            std::cout << std::endl;
        }
    }

private:
    std::string start_name_;
    Rules rules_;
    vvvbool dp_;
    std::string word_;

};

int main(int argc, char **argv)
{
    if (argc != 3) {
        std::cerr << "USAGE: " << argv[0] << " grammar_filepath input_sequence_filepath" << std::endl;
        return 1;
    }
    assert(argc == 3);

    std::ifstream grammar_fin(argv[1]);

    Grammar gr(grammar_fin);
    gr.normalize();
    gr.print_rules();

    std::ifstream fin(argv[2]);
    if (!fin) {
        std::cerr << "Failed to open: " << argv[2] << std::endl;
        return 1;
    }

    std::string word;
    std::getline(fin, word);
    bool result = gr.check(word);
    if (result) {
        gr.show_table();
    }

    return !result;
}

