#pragma once

#include <cstddef>

class Filter
{
public:
    virtual ~Filter() = default;
    virtual void operator()(const float* colorIn, float* colorOut) const = 0;
};
