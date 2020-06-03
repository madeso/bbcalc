#include "calc/calc.h"

#include <string>
#include <sstream>
#include <utility>
#include <vector>
#include <cassert>
#include <memory>
#include <array>
#include <string_view>

#include "calc/str.h"
#include "calc/errorhandler.h"
#include "calc/token.h"
#include "calc/input.h"
#include "calc/lexer.h"
#include "calc/ast.h"
#include "calc/parser.h"


bool
IsCommandLine(char c)
{
    if (c == '/')
    {
        return true;
    }
    if (c == '-')
    {
        return true;
    }
    if (c == '\\')
    {
        return true;
    }
    return false;
}


std::string
ToBinaryString(int n)
{
    if (n == 0)
    {
        return "0";
    }

    std::vector<char> c;
    {
        int number = n;
        int i = 0;
        while (number > 0)
        {
            if (i >= 4)
            {
                i -= 4;
                c.emplace_back(' ');
            }

            c.emplace_back((number & 0x1) != 0 ? '1' : '0');
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


void
PrintNumber(Output* output, int n)
{
    output->PrintInfo(Str{} << "dec: " << std::dec << n);
    output->PrintInfo(Str{} << "hex: 0x" << std::hex << n);
    output->PrintInfo(Str{} << "bin: " << ToBinaryString(n));
}


enum
{
    MainCmdErr = -1,
    MainLexErr = -2,
    MainEmptyLex = -3,
    MainParserErr = -4,
    MainOk = 0,
    MainUsage = 0
};


int
RunCalcApp(
        const std::string& appname,
        const std::vector<std::string>& arguments,
        Output* output)
{
    for (const auto& arg: arguments)
    {
        if (IsCommandLine(arg[0]))
        {
            output->PrintError(Str{} << "Invalid commandline argument " << arg);
            return MainCmdErr;
        }
        else
        {
            ErrorHandler errors;

            const auto tokens = RunLexer(arg, &errors);

            if (errors.HasErr())
            {
                errors.PrintErrors(output);
                return MainLexErr;
            }

            if (tokens.empty())
            {
                output->PrintError("Empty statement");
                return MainEmptyLex;
            }

            auto root = RunParser(tokens, &errors);

            if (errors.HasErr())
            {
                errors.PrintErrors(output);
                return MainParserErr;
            }

            PrintNumber(output, root->Calculate());
        }
    }

    if (arguments.empty())
    {
        output->PrintInfo(appname);
        output->PrintInfo(" - print truth table of expressions");
        output->PrintInfo(" - convert between binary and hexadecimal values");
        output->PrintInfo(" - evaluate boolean expressions");
        return MainUsage;
    }

    return MainOk;
}

