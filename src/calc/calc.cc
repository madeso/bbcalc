#include "calc/calc.h"

#include <string>
#include <sstream>
#include <utility>
#include <vector>
#include <cassert>
#include <memory>
#include <array>
#include <string_view>

#include <fmt/core.h>

#include "calc/errorhandler.h"
#include "calc/token.h"
#include "calc/input.h"
#include "calc/lexer.h"
#include "calc/ast.h"
#include "calc/parser.h"
#include "calc/binary.h"


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


void
PrintNumber(Output* output, int n)
{
    output->PrintInfo(fmt::format("dec: {}", n));
    output->PrintInfo(fmt::format("hex: 0x{:x}", n));
    output->PrintInfo(fmt::format("bin: {}", ToBinaryString(n)));
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
            output->PrintError(fmt::format("Invalid commandline argument {}", arg));
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

