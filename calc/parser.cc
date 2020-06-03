#include "calc/parser.h"

#include "calc/input.h"
#include "calc/str.h"
#include "calc/errorhandler.h"

struct ProvideEofToken
{
    static const Token&
    Provide()
    {
        return Token::Eof();
    }
};


template <typename T>
struct VectorSizeProvider
{
    static int
    Size(const std::vector<T>& vec)
    {
        return ToInt(vec.size());
    }
};


using ParserInput = Input<const Token&,
            std::vector<Token>,
            ProvideEofToken,
            VectorSizeProvider<Token>>;

struct Parser
{
    ParserInput input;

    ErrorHandler* errors;
    explicit Parser(ErrorHandler* e) : errors(e) {}

    std::shared_ptr<Node>
    ParseNumber()
    {
        if (input.Peek().type == Token::NUMBER)
        {
            return std::make_shared<NumberNode>(input.Read().value);
        }
        else
        {
            errors->Err(Str() << "Expected number but got " << input.Read().ToString());
            return ErrorNode::Make();
        }
    }

    std::shared_ptr<Node>
    Parse()
    {
        auto root = ParseNumber();
        if (errors->HasErr())
        {
            return ErrorNode::Make();
        }

        while (!input.IsEof())
        {
            switch (input.Peek().type)
            {
            case Token::OPAND: {
                input.Read();
                auto lhs = root;
                auto rhs = ParseNumber();
                if (errors->HasErr())
                {
                    return ErrorNode::Make();
                }
                root = std::make_shared<AndNode>(lhs, rhs);
                break;
            }
            case Token::OPOR: {
                input.Read();
                auto lhs = root;
                auto rhs = ParseNumber();
                if (errors->HasErr())
                {
                    return ErrorNode::Make();
                }
                root = std::make_shared<OrNode>(lhs, rhs);
                break;
            }
            default:
                errors->Err(Str() << "Expected OP but got " << input.Read().ToString());
                return ErrorNode::Make();
            }
        }

        if (errors->HasErr())
        {
            return ErrorNode::Make();
        }
        else
        {
            return root;
        }
    }
};


std::shared_ptr<Node>
RunParser(const std::vector<Token>& tokens, ErrorHandler* errors)
{
    auto parser = Parser{errors};
    parser.input.input = tokens;
    return parser.Parse();
}

