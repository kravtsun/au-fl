#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <string>
#include <iostream>

#include "rules.h"

struct Grammar {
public:
    Grammar(std::istream &is);

    bool isNormal() const;

    void print_rules();

    void normalize();

    const std::string &start_name() const {
        return start_name_;
    }

    const Rules &rules() const {
        return rules_;
    }
private:
    std::string start_name_;
    Rules rules_;
};

#endif // GRAMMAR_H

