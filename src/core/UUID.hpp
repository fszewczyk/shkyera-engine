#pragma once

#include <stdint.h>

namespace shkyera::UUID {

/**
 * @brief Generate a unique 64-bit UUID.
 *
 * @return A 64-bit UUID value.
 */
uint64_t getUUID();

} // namespace shkyera::UUID
