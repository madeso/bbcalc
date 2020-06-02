#ifndef CALC_ERRORHANDLER_H
#define CALC_ERRORHANDLER_H

#include <string>
#include <vector>


struct Output;


struct ErrorHandler
{
    std::vector<std::string> errors;

    void
    Err(const std::string& str);

    [[nodiscard]] bool
    HasErr() const;

    void
    PrintErrors(Output* output);
};


#endif  // CALC_ERRORHANDLER_H

