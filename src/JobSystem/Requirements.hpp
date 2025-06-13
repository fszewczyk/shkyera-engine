#pragma once

#include <Common/TypeInfo.hpp>

namespace shkyera {

/**
 * Base tag for resources that can be accessed by jobs.
 * All resource types that can be read or written by jobs must inherit from this tag.
 * This ensures type safety and proper resource management in the job system.
 */
struct ResourceTag {};

/**
 * Template variable indicating if a resource type must be accessed only on the main thread.
 * This is used to enforce thread safety for resources that can only be accessed
 * from the main thread.
 * 
 * If a resource requires the main thread, the specialization of that resource type must be true.
 * 
 * @tparam T The resource type to check
 */
template <typename T>
constexpr bool OnlyMainThread = false;

}  // namespace shkyera
