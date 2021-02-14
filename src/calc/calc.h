#ifndef CALC_CALC_H
#define CALC_CALC_H

#include <vector>
#include <string>

#include "calc/output.h"

int
RunCalcApp(
        const std::string& appname,
        const std::vector<std::string>& argument,
        Output* output);

#endif  // CALC_CALC_H
