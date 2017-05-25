#include <algorithm>
#include <cassert>
#include "grammar.h"
#include "grammar_parser.h"
#include "grammar_converter.h"
#include "common.h"

Grammar::Grammar(std::istream &is) {
    std::getline(is, start_name_);

    GrammarParser gp(is);
    while (gp) {
        Rules rules = gp.parse_line();
        std::copy(all(rules), back_inserter(rules_));
    }
}

bool Grammar::isNormal() const {
    int eps_cnt = 0;
    for (auto const &r : rules_) {
        assert(r.left()->isNonTerminal());
        assert(!r.right().empty());

        if (r.right().size() > 2) return false;
        else if (r.right().size() == 1) {
            auto const &t = *r.right().back();
            if (t.isEpsilon()) {
                eps_cnt++;
            }
            else if (t.isNonTerminal()) {
                return false;
            }
        }
        else {
            if (!r.right().front()->isNonTerminal() || !r.right().back()->isNonTerminal()) {
                return false;
            }
        }
    }
    return eps_cnt <= 1;
}

void Grammar::print_rules() {
    auto comp = [](const Rule &l, const Rule &r) -> bool {
        return l.left()->str() < r.left()->str();
    };
    std::vector<Rule> vrules(all(rules_));
    sort(all(vrules), comp);
    std::cout << start_name_ << std::endl;
    forit(rit, vrules) {
        std::string leftstr = rit->left()->str();
        std::cout << leftstr << " =";
        while (true) {
            for (auto const &t : rit->right()) {
                std::cout << " " << *t;
            }
            auto next = rit;
            next++;
            if (next == vrules.end() || next->left()->str() != leftstr) {
                break;
            }
            rit = next;
            std::cout << " |";
        }
        std::cout << std::endl;
    }
}

void Grammar::normalize() {
    GrammarConverter gc{start_name_, rules_};
    gc.convertToChomsky();
    rules_ = gc.rules();
}

const std::string &Grammar::start_name() const {
    return start_name_;
}

const Rules &Grammar::rules() const {
    return rules_;
}
