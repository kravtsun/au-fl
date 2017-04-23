#include "parse_tree.h"

using Node = ParseTree::Node;

Node::Node(const TokenType &token, std::__cxx11::string str, int i, int j)
    : token_(token)
    , str_(str)
    , i_(i)
    , j_(j)
{
    index_ = node_cnt_++;
}

std::ostream &operator<<(std::ostream &os, const Node &rhs) {
    os << "  " << rhs.index_ << " [label=\"<" << rhs.token_->str() << ">\\n" << "(" << rhs.i_ << "," << rhs.j_ << "): " << rhs.str_;
    os << "\"];\n";
    if (rhs.left_) {
        Node::print_edge(os, rhs, *rhs.left_);
        os << *rhs.left_;
    }
    if (rhs.right_) {
        Node::print_edge(os, rhs, *rhs.right_);
        os << *rhs.right_;
    }
    return os;
}

void Node::print_edge(std::ostream &os, const Node &lhs, const Node &rhs) {
    os << "  " << lhs.index_ << " -> " << rhs.index_ << ";\n";
}

void ParseTree::create_node(int i, int j, int k, Node *node) {
    node_map_[make_index(i, j, k)] = static_cast<NodeType>(node);
}

ParseTree::NodeType ParseTree::get_node(int i, int j, int k) {
    return node_map_.at(make_index(i, j, k));
}

void ParseTree::clear() {
    node_cnt_ = 0;
    node_map_.clear();
}

std::ostream &operator<<(std::ostream &os, const ParseTree &pt) {
    os << "digraph ParseTree {\n";
    if (pt.root) {
        os << *pt.root << std::flush;
    }
    os << "}";
    return os;
}

ParseTree::NodeIndexType ParseTree::make_index(int i, int j, int k) {
    return std::make_pair(i, std::make_pair(j, k));
}
