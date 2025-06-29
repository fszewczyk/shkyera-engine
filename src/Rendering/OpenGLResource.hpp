#pragma once

#include <Common/TypeInfo.hpp>
#include <JobSystem/Requirements.hpp>

namespace shkyera {

struct OpenGLResourceTag : public ResourceTag {};

template <>
inline constexpr bool OnlyMainThread<OpenGLResourceTag> = true;

}  // namespace shkyera
