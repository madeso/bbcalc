#include "calc/lexer.h"

#include <cassert>
#include <sstream>

#include "calc/errorhandler.h"
#include "calc/ints.h"
#include "calc/input.h"
#include "calc/str.h"


bool
IsSpace(char c)
{
    if (c == ' ')
    {
        return true;
    }
    if (c == '\t')
    {
        return true;
    }
    if (c == '\n')
    {
        return true;
    }
    if (c == '\r')
    {
        return true;
    }
    return false;
}


bool
IsBinary(char c)
{
    if (c == '0' || c == '1')
    {
        return true;
    }
    else
    {
        return false;
    }
}


bool
IsNumber(char c)
{
    if (c >= '0' && c <= '9')
    {
        return true;
    }
    else
    {
        return false;
    }
}


bool
IsHexa(char c)
{
    if (IsNumber(c))
    {
        return true;
    }
    if ((c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))
    {
        return true;
    }
    return false;
}


bool
IsAz(char c)
{
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
    {
        return true;
    }
    else
    {
        return false;
    }
}


bool
IsAnd(char c)
{
    if (c == '&')
    {
        return true;
    }
    else
    {
        return false;
    }
}


bool
IsOr(char c)
{
    if (c == '|')
    {
        return true;
    }
    else
    {
        return false;
    }
}


int
ParseBinary(const std::string& str)
{
    int n = 0;
    for (const char c: str)
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


int
ParseHexa(const std::string& str)
{
    int n = 0;
    std::istringstream parser(str);
    parser >> std::hex >> n;
    assert(!parser.fail() && "invalid hex?");
    return n;
}


int
ParseDecimal(const std::string& str)
{
    int n = 0;
    std::istringstream parser(str);
    parser >> n;
    assert(!parser.fail() && "invalid decimal?");
    return n;
}


struct ProvideNullChar
{
    static char
    Provide()
    {
        return 0;
    }
};


struct StringSizeProvider
{
    static int
    Size(const std::string& str)
    {
        return ToInt(str.length());
    }
};

using LexerInput = Input<char, std::string, ProvideNullChar, StringSizeProvider>;

struct Lexer
{
    LexerInput input;

    ErrorHandler* errors;
    explicit Lexer(ErrorHandler* e) : errors(e) {}

    void
    SkipSpaces()
    {
        while (!input.IsEof() && IsSpace(input.Peek()))
        {
            input.Read();
        }
    }

    int
    ReadNumber()
    {
        const auto first = input.Peek();
        if (!IsNumber(first))
        {
            errors->Err(Str() << "Numbers must start with a number, but started with '"
                      << first << "' (" << static_cast<int>(first) << ")");
            return 0;
        }
        input.Read();

        const auto second = input.Peek();

        if (second == 'x' || second == 'X')
        {
            input.Read();  // read the x
            std::stringstream ss;
            while (!input.IsEof() && IsHexa(input.Peek()))
            {
                ss << input.Read();
            }
            const auto read = ss.str();
            if (read.empty())
            {
                errors->Err(Str()
                    << "Numbers started with 0x must contain atleast one hexa character but was continued with "
                    << input.Peek());
                return 0;
            }
            return ParseHexa(read);
        }
        else if (second == 'b' || second == 'B')
        {
            input.Read();  // read the b
            std::stringstream ss;
            while (!input.IsEof() && IsNumber(input.Peek()))
            {
                if (IsBinary(input.Peek()))
                {
                    ss << input.Read();
                }
                else
                {
                    errors->Err(Str()
                        << "binary numbers can't contain other than 0 or 1, read: "
                        << input.Peek());
                    return 0;
                }
            }
            const auto read = ss.str();
            if (read.empty())
            {
                errors->Err(Str()
                    << "Numbers started with 0b must contain atleast one binary character but was continued with "
                    << input.Peek());
                return 0;
            }
            return ParseBinary(read);
        }
        else if (IsNumber(second))
        {
            // read the second number
            input.Read();

            std::stringstream ss;
            ss << first << second;

            while (!input.IsEof() && IsNumber(input.Peek()))
            {
                ss << input.Read();
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


    void
    ParseToTokens()
    {
        while (!input.IsEof() && !errors->HasErr())
        {
            SkipSpaces();
            if (!input.IsEof())
            {
                if (IsNumber(input.Peek()))
                {
                    const auto num = ReadNumber();
                    if (!errors->HasErr())
                    {
                        tokens.emplace_back(Token::Number(num));
                    }
                    SkipSpaces();
                }
                else if (IsAnd(input.Peek()))
                {
                    input.Read();
                    tokens.emplace_back(Token::And());
                }
                else if (IsOr(input.Peek()))
                {
                    input.Read();
                    tokens.emplace_back(Token::Or());
                }
                else
                {
                    errors->Err(Str() << "Invalid character: " << input.Peek());
                    return;
                }
            }
        }
    }

    std::vector<Token> tokens;
};


std::vector<Token>
RunLexer(const std::string& source, ErrorHandler* errors)
{
    auto lexer = Lexer{errors};
    lexer.input.input = source;
    lexer.ParseToTokens();
    return lexer.tokens;
}



