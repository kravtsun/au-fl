#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <string>
#include <memory>
#include "token.h"

struct ParseTree {
    struct Node {
        TokenType token;
        Node *left_ = nullptr, *right_ = nullptr;
    };

    Node *root;
};

struct Grammar {
    ParseTree *solve(const std::string &word);
private:
    Rules rules;
    TokenType start;
};

#endif // GRAMMAR_H

