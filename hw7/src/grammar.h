#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <string>

#include "token.h"
#include "grammar_parser.h"
#include "parse_tree.h"

struct Grammar {
    using vbool = std::vector<bool>;
    using vvbool = std::vector<vbool>;
    using vvvbool = std::vector<vvbool>;

public:
    Grammar(std::istream &is);

    void print_rules();

    void normalize();

    bool check(const std::string &word);

    void show_table();

    const ParseTree &tree() {
        return pt_;
    }

private:
    std::string start_name_;
    Rules rules_;
    vvvbool dp_;
    std::string word_;
    ParseTree pt_;
};

#endif // GRAMMAR_H

