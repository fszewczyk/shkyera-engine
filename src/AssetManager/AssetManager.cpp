#include <AssetManager/AssetManager.hpp>

namespace shkyera {

AssetManager& AssetManager::getInstance() {
    static AssetManager manager;
    return manager;
}

}
