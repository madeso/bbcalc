#include "calc/ast.h"


[[nodiscard]] int
ErrorNode::Calculate() const
{
    return 0;
}


std::shared_ptr<Node>
ErrorNode::Make()
{
    return std::make_shared<ErrorNode>();
}


NumberNode::NumberNode(int n) : value(n)
{
}


[[nodiscard]] int
NumberNode::Calculate() const
{
    return value;
}


AndNode::AndNode(std::shared_ptr<Node> l, std::shared_ptr<Node> r)
    : lhs(std::move(l))
    , rhs(std::move(r))
{
}

[[nodiscard]] int
AndNode::Calculate() const
{
    return lhs->Calculate() & rhs->Calculate();
}


OrNode::OrNode(std::shared_ptr<Node> l, std::shared_ptr<Node> r)
    : lhs(std::move(l))
    , rhs(std::move(r))
{
}

[[nodiscard]] int
OrNode::Calculate() const
{
    return lhs->Calculate() | rhs->Calculate();
}


