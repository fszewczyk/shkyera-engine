#pragma once

namespace shkyera {

template <typename AssetType>
class AssetLoader {
 public:
  virtual ~AssetLoader() = default;
  virtual AssetType operator()() = 0;
};

}  // namespace shkyera
