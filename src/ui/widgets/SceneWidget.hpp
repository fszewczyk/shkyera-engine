#pragma once

#include "renderer/Renderer.hpp"
#include "ui/Widget.hpp"

namespace shkyera {

class SceneWidget : public Widget {
  public:
    using Widget::Widget;

    virtual void draw() override;

    void adjustSize();
    void setRenderer(std::shared_ptr<Renderer> renderer);

  private:
    void drawRuntime() const;
    void drawScene();

    std::shared_ptr<Renderer> _renderer;
    ImVec2 _renderSize;
};

} // namespace shkyera
