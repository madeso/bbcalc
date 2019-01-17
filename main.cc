#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cassert>

const char* const PROGRAM_USAGE =
"bintool\n"
" - print truth table of expressions\n"
" - convert between binary and hexadecimal values\n"
" - evaluate boolean expressions\n"
"\n";

struct Str
{
    std::stringstream ss;
    template<typename T> Str& operator<<(const T& t) { ss << t; return *this; }
    std::string str() const { return ss.str(); }
    operator std::string() const { return str(); }
};

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

struct Lexer
{
    std::string input;
    unsigned int next = 0;

    std::vector<std::string> errors;
    void Err(const std::string& str) { errors.emplace_back(str); }
    bool HasErr() const { return !errors.empty(); }

    // read a single char
    char Read()
    {
        if (next >= input.length())
        {
            return 0;
        }
        else
        {
            const auto c = input[next];
            next += 1;
            return c;
        }
    }

    // returns true if we have reached eof and read/peek only return 0
    bool IsEof() const
    {
        return next >= input.length();
    }

    // look ahead 1 character
    char Peek(unsigned int advance=0)
    {
        if (next+advance>= input.length())
        {
            return 0;
        }
        else
        {
            return input[next+advance];
        }
    }

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

            while (!IsEof() && IsBinary(Peek()))
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

    void ParseToList()
    {
        while (!IsEof() && !HasErr())
        {
            SkipSpaces();
            if (!IsEof())
            {
                const auto num = ReadNumber();
                if (!HasErr())
                {
                    numbers.emplace_back(num);
                }
                SkipSpaces();
            }
        }
    }

    std::vector<int> numbers;
};

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

void print_number(int n)
{
    std::cout
        << "dec: " << std::dec << n << "\n"
        << "hex: 0x" << std::hex << n << "\n"
        << "bin: " << ToBinaryString(n) << "\n"
        ;
}

enum
{
    MAIN_CMD_ERR = -1,
    MAIN_LEX_ERR = -2,
    MAIN_EMPTY_LEX = -3,
    MAIN_OK = 0,
    MAIN_USAGE=0
};

int main(int argc, char *argv[])
{
    bool used = false;

    for (int i=1; i<argc; i+=1)
    {
        if (argv[0] == 0) continue;

        const std::string arg = argv[i];
        if (IsCommandLine(arg[0]))
        {
            if (arg.length() <= 1)
            {
                std::cerr << "Invalid commandline argument " << arg << "\n";
                return MAIN_CMD_ERR;
            }
        }
        else
        {
            if (used)
            {
                std::cout << "\n";
            }
            else
            {
                used = true;
            }
            Lexer lexer;
            lexer.input = arg;
            lexer.ParseToList();
            if (lexer.HasErr())
            {
                std::cerr << "Error while parsing: \n";
                for (const auto& err : lexer.errors)
                {
                    std::cerr << " - " << err << "\n";
                }
                return MAIN_LEX_ERR;
            }
            
            if (lexer.numbers.empty())
            {
                std::cerr << "Empty statement\n";
                return MAIN_EMPTY_LEX;
            }

            for (const auto n : lexer.numbers)
            {
                print_number(n);
            }
        }
    }

    if (!used)
    {
        std::cout << PROGRAM_USAGE << "\n";
        return MAIN_USAGE;
    }

    return MAIN_OK;
}
