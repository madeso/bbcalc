#ifndef CALC_STR_H
#define CALC_STR_H

#include <sstream>
#include <string>

struct Str
{
    std::stringstream ss;

    template <typename T>
    Str&
    operator<<(const T& t)
    {
        ss << t;
        return *this;
    }

    operator std::string() const;
};

#endif  // CALC_STR_H

