#pragma once

#include <filesystem>

#include "game/components/ScriptComponent.hpp"
#include "ui/Component.hpp"

namespace shkyera {

class ScriptUIComponent : public UIComponent {
    using UIComponent::UIComponent;

    virtual void draw() override;

  private:
    static constexpr float LABEL_MARGIN = 120.0f;

    void drawScriptFile();
    void drawVariables();

    void drawFloatVariable(PublicFloat &variable);
    void drawIntVariable(PublicInt &variable);
    void drawStringVariable(PublicString &variable);
    void drawVec3Variable(PublicVec3 &variable);

    std::filesystem::path _path;

    std::vector<PublicFloat> _floatVariables;
    std::vector<PublicInt> _intVariables;
    std::vector<PublicString> _stringVariables;
    std::vector<PublicVec3> _vec3Variables;
};

} // namespace shkyera
