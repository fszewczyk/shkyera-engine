#include <ECS/EntityProvider.hpp>

namespace shkyera {

EntityProvider::EntityProvider()
    : _nextEntity(1) {
}

Entity EntityProvider::requestEntity() {
    return _nextEntity++;
}

}  // namespace shkyera
