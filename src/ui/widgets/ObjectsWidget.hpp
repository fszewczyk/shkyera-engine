#pragma once

#include <memory>
#include <vector>

#include "game/Game.hpp"
#include "ui/Widget.hpp"

namespace shkyera {

class ObjectsWidget : public Widget {
  public:
    using Widget::Widget;

    void setGame(std::shared_ptr<Game> game);

    std::shared_ptr<GameObject> getSelectedObject() const;

    virtual void draw() override;

  private:
    void drawCreate();
    void drawList();

    std::shared_ptr<Game> _game;
    std::vector<std::shared_ptr<GameObject>> _gameObjects;
    size_t _selectedGameObject;
};

} // namespace shkyera