#pragma once

#include <vector>

namespace shkyera::simd {

using FloatVec = std::vector<float>;

void add(const FloatVec& src, FloatVec& dst, float subtractBy);

void divide(const FloatVec& dividend, const FloatVec& divisor, FloatVec& dst);

void mix(const FloatVec& a, const FloatVec& b, const FloatVec& t, FloatVec& dst);

void multiplyAccumulate(const FloatVec& a, float b, FloatVec& dst);

void multiplyAccumulate(const FloatVec& a, const FloatVec& b, FloatVec& dst);

}