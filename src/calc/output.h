#ifndef CALC_OUTPUT_H
#define CALC_OUTPUT_H

#include <string>


struct Output
{
    Output();
    virtual ~Output();

    Output(const Output&) = delete;
    Output(Output&&) = delete;
    void
    operator=(const Output&) = delete;
    void
    operator=(Output&&) = delete;

    virtual void
    PrintInfo(const std::string& str) = 0;

    virtual void
    PrintError(const std::string& str) = 0;
};


#endif  // CALC_OUTPUT_H
