#include "calc/token.h"

#include <array>
#include <string_view>
#include <sstream>
#include <cassert>


[[nodiscard]] std::string
Token::ToString() const
{
    constexpr std::array NAMES{
            std::string_view{"NUMBER"},
            std::string_view{"AND"},
            std::string_view{"OR"},
            std::string_view{"EOF"}};
    using A = decltype(NAMES);

    std::stringstream ss;

    ss << NAMES[static_cast<A::size_type>(type)];

    if (type == NUMBER)
    {
        ss << "(" << value << ")";
    }
    return ss.str();
}


Token
Token::Number(int num)
{
    Token ret{};
    ret.type = NUMBER;
    ret.value = num;
    return ret;
}


Token
Token::And()
{
    return FromType(OPAND);
}


Token
Token::Or()
{
    return FromType(OPOR);
}


const Token&
Token::Eof()
{
    static auto Eof = FromType(EOFTOKEN);
    return Eof;
}


Token
Token::FromType(Type t)
{
    assert(t != NUMBER);
    Token ret{};
    ret.type = t;
    ret.value = 0;
    return ret;
}

