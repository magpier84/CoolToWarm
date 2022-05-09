#pragma once

#include "Filter.h"

class sRgb2oRgbFilter : public Filter
{
public:
    void operator()(const float* colorIn, float* colorOut) const override;
};

class oRgb2sRgbFilter : public Filter
{
public:
    void operator()(const float* colorIn, float* colorOut) const override;
};
