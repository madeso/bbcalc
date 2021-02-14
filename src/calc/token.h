#ifndef CALC_TOKEN_H
#define CALC_TOKEN_H

#include <string>


struct Token
{
    enum Type
    {
        NUMBER,
        OPAND,
        OPOR,
        EOFTOKEN
    };

    [[nodiscard]] std::string
    ToString() const;

    Type type;
    int value;

    static Token
    Number(int num);

    static Token
    And();

    static Token
    Or();

    static const Token&
    Eof();

private:
    static Token
    FromType(Type t);
};


#endif  // CALC_TOKEN_H

