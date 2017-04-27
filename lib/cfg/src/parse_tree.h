#ifndef PARSE_TREE_H
#define PARSE_TREE_H

#include <iostream>
#include <map>
#include "token.h"

struct ParseTree {
    struct Node;
    using NodeType = std::shared_ptr<Node> ;
    struct Node {
        Node(const TokenType &token, std::string str, int i, int j);

        NodeType left_ = nullptr, right_ = nullptr;

        friend std::ostream &operator<<(std::ostream &os, const Node &rhs);

    private:
        TokenType token_;
        std::string str_;
        int i_, j_;
        int index_ = -1;

        static void print_edge(std::ostream &os, const Node &lhs, const Node &rhs);
    };

    ParseTree(Node *root)
        : root(root)
    {}

    void create_node(int i, int j, int k, Node *node);

    NodeType get_node(int i, int j, int k);

    void clear();

    friend std::ostream &operator<<(std::ostream &os, const ParseTree &pt);

    NodeType root = nullptr;
private:
    static int node_cnt_;

    using NodeIndexType = std::pair<int, std::pair<int, int>>;
    NodeIndexType make_index(int i, int j, int k);

    std::map<NodeIndexType, NodeType> node_map_;
};

#endif // PARSE_TREE_H
