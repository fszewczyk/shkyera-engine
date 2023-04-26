#include <memory>
#include <vector>

#include "components/Component.hpp"
#include "core/Entity.hpp"

namespace shkyera {

class GameObject : public Entity {
  public:
    void addComponent(std::shared_ptr<Component> component);

  private:
    std::vector<std::shared_ptr<Component>> _components;
};

} // namespace shkyera