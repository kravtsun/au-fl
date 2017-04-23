#include <memory>
#include <string>
#include <sstream>

namespace ast {
    template<typename T>
    void LOG(const T&x) {
        (void)x;
//        std::cerr << x << std::endl;
    }

    typedef struct YYLTYPE
    {
        int first_line;
        int first_column;
        int last_line;
        int last_column;
//        char *str;
    } YYLTYPE;

    struct TripleNode;
    using ptr_type = std::shared_ptr<TripleNode>;

    struct TripleNode {
        TripleNode (TripleNode *left, TripleNode *middle, TripleNode *right, const std::string &label)
            : left(left)
            , middle(middle)
            , right(right)
            , label_(label)
        {
            index = TripleNode::cnt++;
        }

        virtual ~TripleNode() {}

        void print_child(std::ostream &os, ptr_type child) const {
            if (child) {
                os << " " << index << " -> " << child->index << std::endl;
                child->print(os);
            }
        }

        virtual std::string label() const {
            return label_;
        }

        virtual void additional(std::ostream &os) const {
            (void)os;
        }

        virtual void print(std::ostream &os) const {
            os << " " << index << " [label=\"" << label() << "\" ";
            additional(os);
            os << "]" << std::endl;
            print_child(os, left);
            print_child(os, middle);
            print_child(os, right);
        }

        ptr_type left = nullptr, middle = nullptr, right = nullptr;
        std::string label_;

        int index;
        static int cnt;
    };

    struct Node : public TripleNode {
        Node(TripleNode *left, TripleNode *right, const std::string &label)
            : TripleNode(left, nullptr, right, label)
            {}

        virtual ~Node() {}
    };

    int TripleNode::cnt = 0;

    struct LeafNode : public Node {
        LeafNode(const std::string &label, const YYLTYPE &yylloc)
            : Node(nullptr, nullptr, label)
            , lloc(yylloc)
        {}

        std::string label() const override {
            std::ostringstream os;
            os << Node::label() << "\\n";
            os << "line:  " << lloc.first_line   - 1 << "\\n";
            os << "begin: " << lloc.first_column - 1 << "\\n";
            os << "end:   " << lloc.last_column  - 1;
            return os.str();
        }

        YYLTYPE lloc;
    };
} // namespace ast.
