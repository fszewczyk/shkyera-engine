#pragma once

#include <filesystem>

#include "game/components/ScriptComponent.hpp"
#include "ui/UIComponent.hpp"

namespace shkyera {

class ScriptUIComponent : public UIComponent {
    using UIComponent::UIComponent;

    virtual void draw() override;

  private:
    static constexpr float LABEL_MARGIN = 120.0f;

    void replaceScript(std::filesystem::path path);

    void drawScriptFile();
    void drawVariables();

    void drawFloatVariable(PublicFloat &variable);
    void drawIntVariable(PublicInt &variable);
    void drawStringVariable(PublicString &variable);
    void drawVec3Variable(PublicVec3 &variable);

    std::shared_ptr<ScriptComponent> _script;
};

} // namespace shkyera
