#include <iostream>

#include "calc/calc.h"

struct ConsoleOutput : public Output
{
    void PrintInfo(const std::string& str) override
    {
        std::cout << str << "\n";
    }

    void PrintError(const std::string& str) override
    {
        std::cerr << str << "\n";
    }
};


int main(int argc, char *argv[])
{
    std::vector<std::string> arguments;

    for (int i=1; i<argc; i+=1)
    {
        if (argv[0] == 0) continue;
        const std::string arg = argv[i];
        arguments.emplace_back(arg);
    }

    auto console_output = ConsoleOutput{};

    return RunCalcApp(argv[0], arguments, &console_output);
}

