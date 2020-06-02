#include "catch.hpp"

#include "calc/calc.h"

#include "catchy/vectorequals.h"

using namespace catchy;

namespace
{
    struct Line
    {
        bool error;
        std::string text;

        bool operator==(const Line& rhs) const
        {
            return error == rhs.error && text == rhs.text;
        }

    };

    Line Inf(const std::string& s)
    {
        auto l = Line{};
        l.text = s;
        l.error = false;
        return l;
    }

    Line Err(const std::string& s)
    {
        auto l = Line{};
        l.text = s;
        l.error = true;
        return l;
    }

    std::ostream&
    operator<<(std::ostream& stream, const Line& line)
    {
        stream << (line.error ? "ERR" : "INF");
        stream << " " << line.text;
        return stream;
    }

    struct VectorOutput : public Output
    {
        std::vector<Line> lines;

        void PrintInfo(const std::string& str) override
        {
            lines.emplace_back(Inf(str));
        }

        void PrintError(const std::string& str) override
        {
            lines.emplace_back(Err(str));
        }
    };

    catchy::FalseString
    VectorEquals(const VectorOutput& lhs, const std::vector<Line> rhs)
    {
        return catchy::VectorEquals
        (
            lhs.lines,
            rhs,
            [](const Line& f) -> std::string { return Str() << f; },
            [](const Line& a, const Line& b) -> FalseString
            {
                if(a == b)
                {
                    return FalseString::True();
                }
                else
                {
                    return FalseString::False
                    (
                        Str() << a << " != " << b
                    );
                }
            }
        );
    }
}


TEST_CASE("calc-help", "[calc]")
{
    VectorOutput lines;

    SECTION("help")
    {
        const auto output = RunCalcApp("calcapp", {}, &lines);
        CHECK(output == 0);
        CHECK(VectorEquals(lines, {
            Inf("calcapp"),
            Inf(" - print truth table of expressions"),
            Inf(" - convert between binary and hexadecimal values"),
            Inf(" - evaluate boolean expressions")
        }));
    }
}


TEST_CASE("calc-convert", "[calc]")
{
    VectorOutput lines;

    SECTION("dec")
    {
        const auto output = RunCalcApp("calcapp", {"42"}, &lines);
        CHECK(output == 0);
        CHECK(VectorEquals(lines, {
            Inf("dec: 42"),
            Inf("hex: 0x2a"),
            Inf("bin: 10 1010")
        }));
    }

    SECTION("hex")
    {
        const auto output = RunCalcApp("calcapp", {"0x42"}, &lines);
        CHECK(output == 0);
        CHECK(VectorEquals(lines, {
            Inf("dec: 66"),
            Inf("hex: 0x42"),
            Inf("bin: 100 0010")
        }));
    }

    SECTION("binary")
    {
        const auto output = RunCalcApp("calcapp", {"0b1010"}, &lines);
        CHECK(output == 0);
        CHECK(VectorEquals(lines, {
            Inf("dec: 10"),
            Inf("hex: 0xa"),
            Inf("bin: 1010")
        }));
    }
}

