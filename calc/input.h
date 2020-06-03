#ifndef CALC_INPUT_H
#define CALC_INPUT_H

#include "calc/ints.h"


template <typename T, typename C, typename Default, typename SizeProvider>
struct Input
{
    C input;
    int next = 0;

    // read a single char
    T
    Read()
    {
        if (next >= SizeProvider::Size(input))
        {
            return Default::Provide();
        }
        else
        {
            const auto old = next;
            next += 1;
            return input[ToSizet(old)];
        }
    }

    // returns true if we have reached eof and read/peek only return 0
    [[nodiscard]] bool
    IsEof() const
    {
        return next >= SizeProvider::Size(input);
    }

    // look ahead 1 character
    T
    Peek(int advance = 0)
    {
        if (next + advance >= SizeProvider::Size(input))
        {
            return Default::Provide();
        }
        else
        {
            return input[ToSizet(next + advance)];
        }
    }
};


#endif  // CALC_INPUT_H

