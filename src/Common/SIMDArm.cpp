#include <arm_neon.h>

#include <Common/SIMD.hpp>

namespace shkyera::simd {

void add(const FloatVec& src, FloatVec& dst, float add) {
  const auto vectorizedConstant = vdupq_n_f32(add);

  const int simdLength = static_cast<int>(src.size()) - 4;
  int i = 0;
  for (; i < simdLength; i += 4) {
    const auto vectorizedData = vld1q_f32(src.data() + i);
    const auto addedData = vaddq_f32(vectorizedData, vectorizedConstant);
    vst1q_f32(dst.data() + i, addedData);
  }

  for (; i < dst.size(); ++i) {
    dst[i] += add;
  }
}

void divide(const FloatVec& dividend, const FloatVec& divisor, FloatVec& dst) {
  const int simdLength = static_cast<int>(dividend.size()) - 4;

  int i = 0;
  for (; i < simdLength; i += 4) {
    const auto vectorizedDividend = vld1q_f32(dividend.data() + i);
    const auto vectorizedDivisor = vld1q_f32(divisor.data() + i);
    const auto dividedData = vdivq_f32(vectorizedDividend, vectorizedDivisor);
    vst1q_f32(dst.data() + i, dividedData);
  }

  for (; i < divisor.size(); ++i) {
    dst[i] = dividend[i] / divisor[i];
  }
}

void mix(const FloatVec& a, const FloatVec& b, const FloatVec& t, FloatVec& dst) {
  const int simdLength = static_cast<int>(a.size()) - 4;
  const auto vOne = vdupq_n_f32(1.0f);

  int i = 0;
  for (; i < simdLength; i += 4) {
    const auto va = vld1q_f32(a.data() + i);
    const auto vb = vld1q_f32(b.data() + i);
    const auto vt = vld1q_f32(t.data() + i);
    const auto vOneMinusT = vsubq_f32(vOne, vt);
    const auto result = vmlaq_f32(vmulq_f32(vOneMinusT, va), vb, vt);
    vst1q_f32(dst.data() + i, result);
  }

  for (; i < a.size(); ++i) {
    dst[i] = (1.0f - t[i]) * a[i] + t[i] * b[i];
  }
}

void multiplyAccumulate(const FloatVec& a, float b, FloatVec& dst) {
  const auto vb = vdupq_n_f32(b);
  const int simdLength = static_cast<int>(a.size()) - 4;
  int i = 0;
  for (; i < simdLength; i += 4) {
    const auto va = vld1q_f32(a.data() + i);
    auto vDst = vld1q_f32(dst.data() + i);
    vDst = vmlaq_f32(vDst, va, vb);
    vst1q_f32(dst.data() + i, vDst);
  }

  for (; i < a.size(); ++i) {
    dst[i] += a[i] * b;
  }
}

void multiplyAccumulate(const FloatVec& a, const FloatVec& b, FloatVec& dst) {
  const int simdLength = static_cast<int>(a.size()) - 4;
  int i = 0;
  for (; i < simdLength; i += 4) {
    const auto va = vld1q_f32(a.data() + i);
    const auto vb = vld1q_f32(b.data() + i);
    auto vDst = vld1q_f32(dst.data() + i);
    vDst = vmlaq_f32(vDst, va, vb);
    vst1q_f32(dst.data() + i, vDst);
  }

  for (; i < a.size(); ++i) {
    dst[i] += a[i] * b[i];
  }
}

}  // namespace shkyera::simd