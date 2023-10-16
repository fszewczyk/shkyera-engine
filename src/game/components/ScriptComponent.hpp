/**
 * @file ScriptComponent.hpp
 *
 * @brief Contains the declaration of the `ScriptComponent` class and related data structures.
 */

#pragma once

#include "core/Filesystem.hpp"
#include "game/Component.hpp"

namespace shkyera {

/**
 * @brief Structure to represent a public float variable for use in scripts.
 */
typedef struct PublicFloat {
    std::string name; ///< The name of the variable.
    float value;      ///< The value of the variable.
} PublicFloat;

/**
 * @brief Structure to represent a public integer variable for use in scripts.
 */
typedef struct PublicInt {
    std::string name; ///< The name of the variable.
    int value;        ///< The value of the variable.
} PublicInt;

/**
 * @brief Structure to represent a public string variable for use in scripts.
 */
typedef struct PublicString {
    std::string name;  ///< The name of the variable.
    std::string value; ///< The value of the variable.
} PublicString;

/**
 * @brief Structure to represent a public glm::vec3 variable for use in scripts.
 */
typedef struct PublicVec3 {
    std::string name; ///< The name of the variable.
    glm::vec3 value;  ///< The value of the variable.
} PublicVec3;

/**
 * @brief A class representing a script component that can be attached to game objects.
 *
 * This class extends the `Component` class and provides functionality for handling scripts,
 * including adding script variables and managing script files.
 */
class ScriptComponent : public Component {
  public:
    using Component::Component;

    /**
     * @brief Set the script file associated with this script component.
     *
     * @param file A shared pointer to the script file.
     */
    void setFile(std::shared_ptr<File> file);

    /**
     * @brief Update the script component.
     */
    void update();

    /**
     * @brief Move the script associated with this component.
     */
    void moveScript();

    /**
     * @brief Get the script file associated with this script component.
     *
     * @return A shared pointer to the script file.
     */
    const std::shared_ptr<File> getFile() const;

    /**
     * @brief Add a public float variable to the script component.
     *
     * @param name The name of the float variable to add.
     */
    void addFloatVariable(std::string name);

    /**
     * @brief Add a public integer variable to the script component.
     *
     * @param name The name of the integer variable to add.
     */
    void addIntVariable(std::string name);

    /**
     * @brief Add a public string variable to the script component.
     *
     * @param name The name of the string variable to add.
     */
    void addStringVariable(std::string name);

    /**
     * @brief Add a public glm::vec3 variable to the script component.
     *
     * @param name The name of the glm::vec3 variable to add.
     */
    void addVec3Variable(std::string name);

    /**
     * @brief Get a reference to the vector of public float variables.
     *
     * @return A reference to the vector of public float variables.
     */
    std::vector<PublicFloat> &getFloatVariables();

    /**
     * @brief Get a reference to the vector of public integer variables.
     *
     * @return A reference to the vector of public integer variables.
     */
    std::vector<PublicInt> &getIntVariables();

    /**
     * @brief Get a reference to the vector of public string variables.
     *
     * @return A reference to the vector of public string variables.
     */
    std::vector<PublicString> &getStringVariables();

    /**
     * @brief Get a reference to the vector of public glm::vec3 variables.
     *
     * @return A reference to the vector of public glm::vec3 variables.
     */
    std::vector<PublicVec3> &getVec3Variables();

    /**
     * @brief Add a script component to a game object and attach a script file.
     *
     * @param object A shared pointer to the game object.
     * @param file A shared pointer to the script file.
     * @return A shared pointer to the created script component.
     */
    static std::shared_ptr<ScriptComponent> addScript(std::shared_ptr<GameObject> object, std::shared_ptr<File> file);

    /**
     * @brief Add an existing script component to the list of managed scripts.
     *
     * @param script A shared pointer to the script component to add.
     */
    static void addScript(std::shared_ptr<ScriptComponent> script);

    /**
     * @brief Remove a script component from the list of managed scripts.
     *
     * @param script A shared pointer to the script component to remove.
     */
    static void removeScript(std::shared_ptr<ScriptComponent> script);

    /**
     * @brief Get a list of all managed script components.
     *
     * @return A vector of shared pointers to script components.
     */
    static std::vector<std::shared_ptr<ScriptComponent>> getScripts();

    /**
     * @brief Move all scripts to their respective destinations.
     */
    static void moveScripts();

    /**
     * @brief The destination directory for scripts (static member).
     */
    inline static std::string SCRIPT_DESTINATION = "src/python/shkyera";

  private:
    /**
     * @brief Verify the existence of script files and components.
     */
    static void verifyScripts();

    static std::vector<std::shared_ptr<ScriptComponent>> _scripts; ///< List of managed script components.

    std::shared_ptr<File> _file;                ///< The associated script file.
    std::vector<PublicFloat> _floatVariables;   ///< List of public float variables.
    std::vector<PublicInt> _intVariables;       ///< List of public integer variables.
    std::vector<PublicString> _stringVariables; ///< List of public string variables.
    std::vector<PublicVec3> _vec3Variables;     ///< List of public glm::vec3 variables.
};

} // namespace shkyera
