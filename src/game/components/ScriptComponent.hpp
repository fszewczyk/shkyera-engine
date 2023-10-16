#pragma once

#include "core/Filesystem.hpp"
#include "game/Component.hpp"

namespace shkyera {

typedef struct PublicFloat {
    std::string name;
    float value;
} PublicFloat;

typedef struct PublicInt {
    std::string name;
    int value;
} PublicInt;

typedef struct PublicString {
    std::string name;
    std::string value;
} PublicString;

typedef struct PublicVec3 {
    std::string name;
    glm::vec3 value;
} PublicVec3;

class ScriptComponent : public Component {
  public:
    using Component::Component;

    void setFile(std::shared_ptr<File> file);
    void update();
    void moveScript();

    const std::shared_ptr<File> getFile() const;

    void addFloatVariable(std::string name);
    void addIntVariable(std::string name);
    void addStringVariable(std::string name);
    void addVec3Variable(std::string name);

    std::vector<PublicFloat> &getFloatVariables();
    std::vector<PublicInt> &getIntVariables();
    std::vector<PublicString> &getStringVariables();
    std::vector<PublicVec3> &getVec3Variables();

    static std::shared_ptr<ScriptComponent> addScript(std::shared_ptr<GameObject> object, std::shared_ptr<File> file);
    static void addScript(std::shared_ptr<ScriptComponent> script);
    static void removeScript(std::shared_ptr<ScriptComponent> script);
    static std::vector<std::shared_ptr<ScriptComponent>> getScripts();

    static void moveScripts();

    inline static std::string SCRIPT_DESTINATION = "src/python/shkyera";

  private:
    static void verifyScripts();

    static std::vector<std::shared_ptr<ScriptComponent>> _scripts;

    std::shared_ptr<File> _file;
    std::vector<PublicFloat> _floatVariables;
    std::vector<PublicInt> _intVariables;
    std::vector<PublicString> _stringVariables;
    std::vector<PublicVec3> _vec3Variables;
};

} // namespace shkyera
