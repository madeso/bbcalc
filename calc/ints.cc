#include "calc/ints.h"


int
ToInt(std::size_t i)
{
    return static_cast<int>(i);
}


std::size_t
ToSizet(int i)
{
    return static_cast<std::size_t>(i);
}

