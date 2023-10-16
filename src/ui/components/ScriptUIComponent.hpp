/**
 * @file ScriptUIComponent.hpp
 *
 * @brief Contains the declaration of the `ScriptUIComponent` class, representing a user interface component for
 * scripting.
 */

#pragma once

#include <filesystem>

#include "game/components/ScriptComponent.hpp"
#include "ui/UIComponent.hpp"

namespace shkyera {

/**
 * @brief A class representing a user interface component for interacting with scripts.
 *
 * The `ScriptUIComponent` class is a specialized UI component used to display and manage script-related information,
 * including script files and their associated variables.
 */
class ScriptUIComponent : public UIComponent {
    using UIComponent::UIComponent;

    /**
     * @brief Implementation of the abstract `draw` method to render the UI for this component.
     */
    virtual void draw() override;

  private:
    /**
     * @brief The margin used for labeling elements in the UI.
     */
    static constexpr float LABEL_MARGIN = 120.0f;

    /**
     * @brief Replace the current script with a new one from the specified file path.
     *
     * @param path The path to the new script file.
     */
    void replaceScript(std::filesystem::path path);

    /**
     * @brief Draw the script file and its contents in the UI.
     */
    void drawScriptFile();

    /**
     * @brief Draw the script variables in the UI.
     */
    void drawVariables();

    /**
     * @brief Draw a float variable in the UI.
     *
     * @param variable The float variable to render.
     */
    void drawFloatVariable(PublicFloat &variable);

    /**
     * @brief Draw an integer variable in the UI.
     *
     * @param variable The integer variable to render.
     */
    void drawIntVariable(PublicInt &variable);

    /**
     * @brief Draw a string variable in the UI.
     *
     * @param variable The string variable to render.
     */
    void drawStringVariable(PublicString &variable);

    /**
     * @brief Draw a glm::vec3 variable in the UI.
     *
     * @param variable The glm::vec3 variable to render.
     */
    void drawVec3Variable(PublicVec3 &variable);

    std::shared_ptr<ScriptComponent> _script; ///< A shared pointer to the associated script component.
};

} // namespace shkyera
