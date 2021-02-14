#ifndef CALC_AST_H
#define CALC_AST_H

#include <memory>


struct Node
{
    Node() = default;
    virtual ~Node() = default;

    Node(const Node&) = delete;
    Node(Node&&) = delete;
    void
    operator=(const Node&) = delete;
    void
    operator=(Node&&) = delete;

    [[nodiscard]] virtual int
    Calculate() const = 0;
};


struct ErrorNode : public Node
{
    [[nodiscard]] int
    Calculate() const override;

    static std::shared_ptr<Node>
    Make();
};


struct NumberNode : public Node
{
    int value;

    explicit NumberNode(int n);

    [[nodiscard]] int
    Calculate() const override;
};


struct AndNode : public Node
{
    std::shared_ptr<Node> lhs;
    std::shared_ptr<Node> rhs;

    AndNode(std::shared_ptr<Node> l, std::shared_ptr<Node> r);

    [[nodiscard]] int
    Calculate() const override;
};


struct OrNode : public Node
{
    std::shared_ptr<Node> lhs;
    std::shared_ptr<Node> rhs;

    OrNode(std::shared_ptr<Node> l, std::shared_ptr<Node> r);

    [[nodiscard]] int
    Calculate() const override;
};


#endif  // CALC_AST_H

