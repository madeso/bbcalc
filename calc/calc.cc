#include "calc/calc.h"

#include <string>
#include <sstream>
#include <vector>
#include <cassert>
#include <memory>

// ----------------------------------------------------------------------------------------------------
// Util related
// ----------------------------------------------------------------------------------------------------

Output::~Output() {}

int to_int(std::size_t i)
{
    return static_cast<int>(i);
}

std::size_t to_sizet(int i)
{
    return static_cast<std::size_t>(i);
}

std::string Str::str() const { return ss.str(); }

Str::operator std::string() const { return str(); }

struct ErrorHandler
{
    std::vector<std::string> errors;
    void Err(const std::string& str) { errors.emplace_back(str); }
    bool HasErr() const { return !errors.empty(); }

    void PrintErrors(Output* output)
    {
        output->PrintError("Error while parsing:");
        for (const auto& err : errors)
        {
            output->PrintError(Str{} << " - " << err);
        }
    }
};

template<typename T, typename C, typename Default, typename SizeProvider>
struct Input
{
    C input;
    int next = 0;

    // read a single char
    T Read()
    {
        if (next >= SizeProvider::Size(input))
        {
            return Default::Provide();
        }
        else
        {
            const auto old = next;
            next += 1;
            return input[to_sizet(old)];
        }
    }

    // returns true if we have reached eof and read/peek only return 0
    bool IsEof() const
    {
        return next >= SizeProvider::Size(input);
    }

    // look ahead 1 character
    T Peek(int advance = 0)
    {
        if (next + advance >= SizeProvider::Size(input))
        {
            return Default::Provide();
        }
        else
        {
            return input[to_sizet(next + advance)];
        }
    }
};

// ----------------------------------------------------------------------------------------------------
// Common related
// ----------------------------------------------------------------------------------------------------

struct Token
{
    enum Type { NUMBER, OPAND, OPOR, EOFTOKEN };

    std::string ToString() const
    {
        static const char* NAMES[] = { "NUMBER", "AND", "OR", "EOF" };
        std::stringstream ss;

        ss << NAMES[static_cast<int>(type)];

        if(type == NUMBER)
        {
            ss << "(" << value << ")";
        }
        return ss.str();
    }

    Type type;
    int value;

    static Token Number(int num) {
        Token ret;
        ret.type = NUMBER;
        ret.value = num;
        return ret;
    }

    static Token And() { return FromType(OPAND); }
    static Token Or() { return FromType(OPOR); }

    static const Token& Eof() { static auto eof = FromType(EOFTOKEN); return eof; }

private:
    static Token FromType(Type t)
    {
        assert(t != NUMBER);
        Token ret;
        ret.type = t;
        ret.value = 0;
        return ret;
    }
};

// ----------------------------------------------------------------------------------------------------
// Lexing related
// ----------------------------------------------------------------------------------------------------

bool IsSpace(char c)
{
    if (c == ' ') return true;
    if (c == '\t') return true;
    if (c == '\n') return true;
    if (c == '\r') return true;
    return false;
}

bool IsBinary(char c)
{
    if (c == '0' || c == '1') return true;
    else return false;
}

bool IsNumber(char c)
{
    if (c >= '0' && c <= '9') return true;
    else return false;
}

bool IsHexa(char c)
{
    if (IsNumber(c)) return true;
    if ((c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')) return true;
    return false;
}

bool IsAz(char c)
{
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) return true;
    else return false;
}

bool IsAnd(char c)
{
    if (c == '&') return true;
    else return false;
}

bool IsOr(char c)
{
    if (c == '|') return true;
    else return false;
}

int ParseBinary(const std::string& str)
{
    int n = 0;
    for (const char c : str)
    {
        n = n << 1;
        if (c == '1')
        {
            n = n | 0x1;
        }
        else if (c == '0')
        {
            // nothing
        }
        else
        {
            assert(0 && "invalid binary");
        }
    }
    return n;
}

int ParseHexa(const std::string& str)
{
    int n;
    std::istringstream parser(str);
    parser >> std::hex >> n;
    assert(!parser.fail() && "invalid hex?");
    return n;
}

int ParseDecimal(const std::string& str)
{
    int n;
    std::istringstream parser(str);
    parser >> n;
    assert(!parser.fail() && "invalid decimal?");
    return n;
}

struct ProvideNullChar
{
    static char Provide()
    {
        return 0;
    }
};

struct StringSizeProvider
{
    static int Size(const std::string& str)
    {
        return to_int(str.length());
    }
};

struct Lexer : public ErrorHandler, public Input<char, std::string, ProvideNullChar, StringSizeProvider>
{
    void SkipSpaces()
    {
        while (!IsEof() && IsSpace(Peek()))
        {
            Read();
        }
    }

    int ReadNumber()
    {
        const auto first = Peek();
        if (!IsNumber(first))
        {
            Err(Str() << "Numbers must start with a number, but started with '" << first << "' (" << static_cast<int>(first) << ")");
            return 0;
        }
        Read();

        const auto second = Peek();
        
        if(second == 'x' || second == 'X')
        {
            Read(); // read the x
            std::stringstream ss;
            while (!IsEof() && IsHexa(Peek()))
            {
                ss << Read();
            }
            const auto read = ss.str();
            if (read.empty())
            {
                Err(Str() << "Numbers started with 0x must contain atleast one hexa character but was continued with " << Peek());
                return 0;
            }
            return ParseHexa(read);
        }
        else if(second == 'b' || second == 'B')
        {
            Read(); // read the b
            std::stringstream ss;
            while (!IsEof() && IsNumber(Peek()))
            {
                if(IsBinary(Peek()))
                {
                    ss << Read();
                }
                else
                {
                    Err(Str() << "binary numbers can't contain other than 0 or 1, read: " << Peek());
                    return 0;
                }
            }
            const auto read = ss.str();
            if (read.empty())
            {
                Err(Str() << "Numbers started with 0b must contain atleast one binary character but was continued with " << Peek());
                return 0;
            }
            return ParseBinary(read);
        }
        else if (IsNumber(second))
        {
            // read the second number
            Read();

            std::stringstream ss;
            ss << first << second;

            while (!IsEof() && IsNumber(Peek()))
            {
                ss << Read();
            }
            const auto read = ss.str();
            return ParseDecimal(read);
        }
        else
        {
            // single character decimal or octal number
            return first - '0';
        }
    }

    void ParseToTokens()
    {
        while (!IsEof() && !HasErr())
        {
            SkipSpaces();
            if (!IsEof())
            {
                if (IsNumber(Peek()))
                {
                    const auto num = ReadNumber();
                    if (!HasErr())
                    {
                        tokens.emplace_back(Token::Number(num));
                    }
                    SkipSpaces();
                }
                else if (IsAnd(Peek()))
                {
                    Read();
                    tokens.emplace_back(Token::And());
                }
                else if (IsOr(Peek()))
                {
                    Read();
                    tokens.emplace_back(Token::Or());
                }
                else
                {
                    Err(Str() << "Invalid character: " << Peek());
                    return;
                }
            }
        }
    }

    std::vector<Token> tokens;
};

// ----------------------------------------------------------------------------------------------------
// Parsing related
// ----------------------------------------------------------------------------------------------------

struct Node
{
    virtual ~Node() {}
    virtual int Calculate() const = 0;
};

struct ErrorNode : public Node
{
    int Calculate() const override { return 0; }
    static std::shared_ptr<Node> Make() { return std::make_shared<ErrorNode>(); }
};


struct NumberNode : public Node
{
    int value;

    NumberNode(int n) : value(n) {}
    int Calculate() const override { return value; }
};

struct AndNode : public Node
{
    std::shared_ptr<Node> lhs;
    std::shared_ptr<Node> rhs;

    AndNode(std::shared_ptr<Node> l, std::shared_ptr<Node> r) : lhs(l), rhs(r) {}
    int Calculate() const override { return lhs->Calculate() & rhs->Calculate(); }
};

struct OrNode : public Node
{
    std::shared_ptr<Node> lhs;
    std::shared_ptr<Node> rhs;

    OrNode(std::shared_ptr<Node> l, std::shared_ptr<Node> r) : lhs(l), rhs(r) {}
    int Calculate() const override { return lhs->Calculate() | rhs->Calculate(); }
};

struct ProvideEofToken
{
    static const Token& Provide() { return Token::Eof(); }
};

template<typename T>
struct VectorSizeProvider
{
    static int Size(const std::vector<T>& vec)
    {
        return to_int(vec.size());
    }
};

struct Parser : public ErrorHandler, Input<const Token&, std::vector<Token>, ProvideEofToken, VectorSizeProvider<Token> >
{
    std::shared_ptr<Node> ParseNumber()
    {
        if (Peek().type == Token::NUMBER)
        {
            return std::make_shared<NumberNode>(Read().value);
        }
        else
        {
            Err(Str() << "Expected number but got " << Read().ToString());
            return ErrorNode::Make();
        }
    }

    std::shared_ptr<Node> Parse()
    {
        auto root = ParseNumber();
        if (HasErr()) return ErrorNode::Make();

        while (!IsEof())
        {
            switch (Peek().type)
            {
            case Token::OPAND:
            {
                Read();
                auto lhs = root;
                auto rhs = ParseNumber();
                if (HasErr()) return ErrorNode::Make();
                root = std::make_shared<AndNode>(lhs, rhs);
                break;
            }
            case Token::OPOR:
            {
                Read();
                auto lhs = root;
                auto rhs = ParseNumber();
                if (HasErr()) return ErrorNode::Make();
                root = std::make_shared<OrNode>(lhs, rhs);
                break;
            }
            default:
                Err(Str() << "Expected OP but got " << Read().ToString());
                return ErrorNode::Make();
            }
        }

        if (HasErr()) return ErrorNode::Make();
        else return root;
    }
};

// ----------------------------------------------------------------------------------------------------
// Commandline related
// ----------------------------------------------------------------------------------------------------

bool IsCommandLine(char c)
{
    if (c == '/') return true;
    if (c == '-') return true;
    if (c == '\\') return true;
    return false;
}

std::string ToBinaryString(int n)
{
    if (n == 0) { return "0"; }

    std::vector<char> c;
    {
        int number = n;
        int i = 0;
        while (number > 0)
        {
            if (i >= 4) {
                i -= 4;
                c.emplace_back(' ');
            }

            c.emplace_back((number & 0x1) ? '1' : '0');
            i += 1;
            number = number >> 1;
        }
    }

    std::stringstream ss;
    for (auto i = c.rbegin(); i != c.rend(); i++)
    {
        ss << *i;
    }
    return ss.str();
}

void print_number(Output* output, int n)
{
    output->PrintInfo(Str{} << "dec: " << std::dec << n);
    output->PrintInfo(Str{} << "hex: 0x" << std::hex << n);
    output->PrintInfo(Str{} << "bin: " << ToBinaryString(n));
}

enum
{
    MAIN_CMD_ERR = -1,
    MAIN_LEX_ERR = -2,
    MAIN_EMPTY_LEX = -3,
    MAIN_PARSER_ERR = -4,
    MAIN_OK = 0,
    MAIN_USAGE=0
};

int RunCalcApp
(
    const std::string& appname,
    const std::vector<std::string>& arguments,
    Output* output
)
{
    for(const auto& arg: arguments)
    {
        if (IsCommandLine(arg[0]))
        {
            if (arg.length() <= 1)
            {
                output->PrintError(Str{} << "Invalid commandline argument " << arg);
                return MAIN_CMD_ERR;
            }
        }
        else
        {
            Lexer lexer;
            lexer.input = arg;
            lexer.ParseToTokens();
            if (lexer.HasErr())
            {
                lexer.PrintErrors(output);
                return MAIN_LEX_ERR;
            }

            if (lexer.tokens.empty())
            {
                output->PrintError("Empty statement");
                return MAIN_EMPTY_LEX;
            }

            Parser parser;
            parser.input = lexer.tokens;

            auto root = parser.Parse();

            if (parser.HasErr())
            {
                parser.PrintErrors(output);
                return MAIN_PARSER_ERR;
            }

            print_number(output, root->Calculate());
        }
    }

    if (arguments.empty())
    {
        output->PrintInfo(appname);
        output->PrintInfo(" - print truth table of expressions");
        output->PrintInfo(" - convert between binary and hexadecimal values");
        output->PrintInfo(" - evaluate boolean expressions");
        return MAIN_USAGE;
    }

    return MAIN_OK;
}

