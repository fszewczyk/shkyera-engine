#include <set>

#include "game/components/ScriptComponent.hpp"
#include "python/StaticAnalysis.hpp"

namespace shkyera {

void ScriptComponent::setFile(std::shared_ptr<File> file) {
    _file = file;
    update();
}

void ScriptComponent::update() {
    _intVariables.clear();

    std::vector<std::pair<std::string, Python::PYTHON_TYPE>> publicVars =
        Python::getPublicVariables(_file->getNameWithoutExtension());

    for (auto [name, type] : publicVars) {
        _intVariables.push_back({name, 0});
        /// TODO: Implement more types
    }
}

const std::shared_ptr<File> ScriptComponent::getFile() const { return _file; }

void ScriptComponent::addFloatVariable(std::string name) { _floatVariables.push_back({name, 0}); }
void ScriptComponent::addIntVariable(std::string name) { _intVariables.push_back({name, 0}); }
void ScriptComponent::addStringVariable(std::string name) { _stringVariables.push_back({name, ""}); }
void ScriptComponent::addVec3Variable(std::string name) { _vec3Variables.push_back({name, {0, 0, 0}}); }

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

std::vector<std::shared_ptr<ScriptComponent>> ScriptComponent::getScripts() { return _scripts; }

void ScriptComponent::moveScripts() {
    verifyScripts();
    for (auto script : _scripts) {
        auto name = script->getFile()->getName();
        auto sourcePath = script->getFile()->getPath();
        auto destinationPath = std::filesystem::path(SCRIPT_DESTINATION) / name;

        if (destinationPath != sourcePath)
            std::filesystem::copy(sourcePath, destinationPath, std::filesystem::copy_options::overwrite_existing);
    }
}

void ScriptComponent::verifyScripts() {
    std::vector<std::shared_ptr<File>> scannedFiles;
    std::vector<std::string> scannedFileNames;
    for (auto script : _scripts) {
        auto file = script->getFile();
        auto name = file->getName();

        if (std::find(scannedFiles.begin(), scannedFiles.end(), file) == scannedFiles.end()) {
            if (std::find(scannedFileNames.begin(), scannedFileNames.end(), name) != scannedFileNames.end()) {
                throw "Looks like you have two scripts with the same name: " + name;
            }
        }

        scannedFiles.push_back(file);
        scannedFileNames.push_back(name);
    }
}

std::vector<std::shared_ptr<ScriptComponent>> ScriptComponent::_scripts = {};

} // namespace shkyera
