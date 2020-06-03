#include "calc/binary.h"

#include <vector>
#include <sstream>


std::string
ToBinaryString(int n)
{
    if (n == 0)
    {
        return "0";
    }

    std::vector<char> c;
    {
        int number = n;
        int i = 0;
        while (number > 0)
        {
            if (i >= 4)
            {
                i -= 4;
                c.emplace_back(' ');
            }

            c.emplace_back((number & 0x1) != 0 ? '1' : '0');
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

