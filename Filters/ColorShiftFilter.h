#pragma once

#include "Filter.h"

class ColorShiftFilter : public Filter
{
public:
    ColorShiftFilter(const float blueYellow, const float redGreen);

    void operator ()(const float* colorIn, float* colorOut) const override;

private:
    float _blueYellow;
    float _redGreen;
};
