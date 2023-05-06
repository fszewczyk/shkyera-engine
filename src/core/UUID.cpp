#include "core/UUID.hpp"

namespace shkyera::UUID {

uint64_t _counter = 0;

uint64_t getUUID() { return _counter++; }

} // namespace shkyera::UUID