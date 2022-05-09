#include "ColorShiftFilter.h"

ColorShiftFilter::ColorShiftFilter(const float blueYellow, const float redGreen)
    : _blueYellow(blueYellow)
    , _redGreen(redGreen)
{
}

void ColorShiftFilter::operator ()(const float* colorIn, float* colorOut) const
{
    colorOut[0] = colorIn[0];
    colorOut[1] = colorIn[1] + _blueYellow;
    colorOut[2] = colorIn[2] + _redGreen;
}
