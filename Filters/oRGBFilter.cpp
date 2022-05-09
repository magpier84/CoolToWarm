#include "oRGBFilter.h"

#include <cmath>

#include <iostream>

namespace
{
static const double pi = std::acos(-1);

template<class T>
constexpr const T& clamp(const T& v, const T& lo, const T& hi)
{
    return v < lo ? lo : hi < v ? hi : v;
}

}

void sRgb2oRgbFilter::operator()(const float* colorIn, float* colorOut) const
{
    const auto R = colorIn[0];
    const auto G = colorIn[1];
    const auto B = colorIn[2];

    const auto L  = 0.2990 * R + 0.5870 * G + 0.1140 * B;
    const auto C1 = 0.5000 * R + 0.5000 * G - 1.0000 * B;
    const auto C2 = 0.8660 * R - 0.8660 * G + 0.0000 * B;

    const auto tetha = std::atan2(C2, C1);
    const auto tethaM = std::abs(tetha);
    const auto tetha0 = std::copysign(tethaM < pi / 3 ? 3 * tethaM / 2
                                                      : pi / 2 + 3 * (tethaM - pi / 3) / 4, tetha);
    const auto angle = tetha0 - tetha;

    const auto Cyb = C1 * std::cos(angle) - C2 * std::sin(angle);
    const auto Crg = C1 * std::sin(angle) + C2 * std::cos(angle);

    colorOut[0] = L;
    colorOut[1] = Cyb;
    colorOut[2] = Crg;
}

void oRgb2sRgbFilter::operator()(const float* colorIn, float* colorOut) const
{
    const auto L   = colorIn[0];
    const auto Cyb = colorIn[1];
    const auto Crg = colorIn[2];

    const auto tetha0 = std::atan2(Crg, Cyb);
    const auto tethaM = std::abs(tetha0);
    const auto tetha = std::copysign(tethaM < pi / 2 ? 2 * tethaM / 3
                                                     : pi / 3 + 4 * (tethaM - pi / 2) / 3, tetha0);
    const auto angle = tetha - tetha0;

    const auto C1 = Cyb * std::cos(angle) - Crg * std::sin(angle);
    const auto C2 = Cyb * std::sin(angle) + Crg * std::cos(angle);

    colorOut[0] = clamp(1.0000 * L + 0.1140 * C1 + 0.7436 * C2, 0.0, 1.0);
    colorOut[1] = clamp(1.0000 * L + 0.1140 * C1 - 0.4111 * C2, 0.0, 1.0);
    colorOut[2] = clamp(1.0000 * L - 0.8860 * C1 + 0.1663 * C2, 0.0, 1.0);
}
