#ifndef CALC_CALC_H
#define CALC_CALC_H

#include <vector>
#include <string>

struct Output
{
    virtual ~Output();

    virtual void PrintInfo(const std::string& str) = 0;
    virtual void PrintError(const std::string& str) = 0;
};


int RunCalcApp
(
    const std::string& appname,
    const std::vector<std::string>& argument,
    Output* output
);

#endif  // CALC_CALC_H

