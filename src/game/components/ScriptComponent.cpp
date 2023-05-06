#include "game/components/ScriptComponent.hpp"

namespace shkyera {

void ScriptComponent::setFile(std::shared_ptr<File> file) {
    _file = file;
    update();
}

void ScriptComponent::update() {
    _floatVariables.push_back({"Floating", 4.0f});
    _intVariables.push_back({"Integer", 4});
    _stringVariables.push_back({"Text", ""});
    _vec3Variables.push_back({"Vector 3", {0, 0, 0}});
}

const std::shared_ptr<File> ScriptComponent::getFile() const { return _file; }

std::vector<PublicFloat> &ScriptComponent::getFloatVariables() { return _floatVariables; }
std::vector<PublicInt> &ScriptComponent::getIntVariables() { return _intVariables; }
std::vector<PublicString> &ScriptComponent::getStringVariables() { return _stringVariables; }
std::vector<PublicVec3> &ScriptComponent::getVec3Variables() { return _vec3Variables; }

std::shared_ptr<ScriptComponent> ScriptComponent::addScript(std::shared_ptr<GameObject> object,
                                                            std::shared_ptr<File> file) {
    auto script = std::make_shared<ScriptComponent>(object);
    script->setFile(file);
    addScript(script);
    return script;
}
void ScriptComponent::addScript(std::shared_ptr<ScriptComponent> script) { _scripts.push_back(script); }
void ScriptComponent::removeScript(std::shared_ptr<ScriptComponent> script) { std::erase(_scripts, script); }

std::vector<std::shared_ptr<ScriptComponent>> ScriptComponent::_scripts = {};

} // namespace shkyera