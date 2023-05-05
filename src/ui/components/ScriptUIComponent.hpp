#pragma once

#include <filesystem>

#include "ui/Component.hpp"

namespace shkyera {

class ScriptUIComponent : public UIComponent {
    using UIComponent::UIComponent;

    virtual void draw() override;

  private:
    void drawScriptFile();

    std::filesystem::path _path;
};

} // namespace shkyera
