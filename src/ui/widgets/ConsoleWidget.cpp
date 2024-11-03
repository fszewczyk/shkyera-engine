#include <iostream>

#include <imgui_internal.h>
#include "imgui.h"

#include <AssetManager/AssetManager.hpp>
#include <AssetManager/Image.hpp>
#include <UI/Widgets/ConsoleWidget.hpp>

namespace shkyera {

Log::Log(const std::string& content) : _content(content) {}

void Log::draw() const {
  ImGui::Image((ImTextureID)getIconId(), ImVec2(24, 24));
  ImGui::SameLine();
  ImGui::TextUnformatted(_content.c_str());
  ImGui::Separator();
}

void* Log::getIconId() const {
  return _icon->getImguiTextureID();
}

LogVerbose::LogVerbose(const std::string& content) : Log(content)
{
  _icon = AssetManager::getInstance().getAsset<Texture>(Image::ICON_CONSOLE_VERBOSE);
}

LogInfo::LogInfo(const std::string& content) : Log(content)
{
  _icon = AssetManager::getInstance().getAsset<Texture>(Image::ICON_CONSOLE_INFO);
}

LogSuccess::LogSuccess(const std::string& content) : Log(content)
{
  _icon = AssetManager::getInstance().getAsset<Texture>(Image::ICON_CONSOLE_SUCCESS);
}

LogError::LogError(const std::string& content) : Log(content)
{
  _icon = AssetManager::getInstance().getAsset<Texture>(Image::ICON_CONSOLE_ERROR);
}

ConsoleWidget::ConsoleWidget(const std::string& name) : Widget(name)
{
  _verboseIcon = AssetManager::getInstance().getAsset<Texture>(Image::ICON_CONSOLE_VERBOSE);
  _infoIcon = AssetManager::getInstance().getAsset<Texture>(Image::ICON_CONSOLE_INFO);
  _successIcon = AssetManager::getInstance().getAsset<Texture>(Image::ICON_CONSOLE_SUCCESS);
  _errorIcon = AssetManager::getInstance().getAsset<Texture>(Image::ICON_CONSOLE_ERROR);
  _totalIcon = AssetManager::getInstance().getAsset<Texture>(Image::ICON_CONSOLE_ERROR);
}


void ConsoleWidget::draw() {
  ImGui::Begin(_name.c_str());

  drawInfoBar();

  ImGui::Separator();

  std::vector<std::shared_ptr<Log>>::iterator log = _logs.end();
  while (log != _logs.begin()) {
    --log;
    (*log)->draw();
  }

  ImGui::End();
}

void ConsoleWidget::drawInfoBar() const {
  if (ImGui::Button("Clear"))
    clear();

  ImGui::SameLine();
  ImGui::Image(_totalIcon->getImguiTextureID(),
               ImVec2(22, 22));
  ImGui::SameLine();
  ImGui::TextUnformatted((std::to_string(_logs.size()) + " Total").c_str());
  ImGui::SameLine();
  ImGui::Dummy(ImVec2(15, 0));
  ImGui::SameLine();
  ImGui::Image(_errorIcon->getImguiTextureID(),
               ImVec2(22, 22));
  ImGui::SameLine();
  ImGui::TextUnformatted((std::to_string(_totalError) + " Error").c_str());
  ImGui::SameLine();
  ImGui::Dummy(ImVec2(15, 0));
  ImGui::SameLine();
  ImGui::Image(_successIcon->getImguiTextureID(),
               ImVec2(22, 22));
  ImGui::SameLine();
  ImGui::TextUnformatted((std::to_string(_totalSuccess) + " Success").c_str());
  ImGui::SameLine();
  ImGui::Dummy(ImVec2(15, 0));
  ImGui::SameLine();
  ImGui::Image(_infoIcon->getImguiTextureID(),
               ImVec2(22, 22));
  ImGui::SameLine();
  ImGui::TextUnformatted((std::to_string(_totalInfo) + " Info").c_str());
  ImGui::SameLine();
  ImGui::Dummy(ImVec2(15, 0));
  ImGui::SameLine();
  ImGui::Image(_verboseIcon->getImguiTextureID(),
               ImVec2(22, 22));
  ImGui::SameLine();
  ImGui::TextUnformatted((std::to_string(_totalVerbose) + " Verbose").c_str());
}

void ConsoleWidget::clear() {
  _logs.clear();

  _totalVerbose = 0;
  _totalSuccess = 0;
  _totalInfo = 0;
  _totalError = 0;
}

void ConsoleWidget::enableVerbose() {
  _verbose = true;
}
void ConsoleWidget::disableVerbose() {
  _verbose = false;
}

void ConsoleWidget::logVerbose(std::string text) {
  size_t logSize = _totalVerbose + _totalSuccess + _totalInfo + _totalError;

  if (_verbose && logSize < MAX_LOGS) {
    _logs.push_back(std::make_shared<LogVerbose>(text));
    _totalVerbose++;
  }
}
void ConsoleWidget::logInfo(std::string text) {
  size_t logSize = _totalVerbose + _totalSuccess + _totalInfo + _totalError;

  if (logSize < MAX_LOGS) {
    _logs.push_back(std::make_shared<LogInfo>(text));
    _totalInfo++;
  }
}
void ConsoleWidget::logSuccess(std::string text) {
  size_t logSize = _totalVerbose + _totalSuccess + _totalInfo + _totalError;

  if (logSize < MAX_LOGS) {
    _logs.push_back(std::make_shared<LogSuccess>(text));
    _totalSuccess++;
  }
}
void ConsoleWidget::logError(std::string text) {
  size_t logSize = _totalVerbose + _totalSuccess + _totalInfo + _totalError;

  if (logSize < MAX_LOGS) {
    _logs.push_back(std::make_shared<LogError>(text));
    _totalError++;
  }
}

std::vector<std::shared_ptr<Log>> ConsoleWidget::_logs = {};
bool ConsoleWidget::_verbose = true;

size_t ConsoleWidget::_totalVerbose = 0;
size_t ConsoleWidget::_totalInfo = 0;
size_t ConsoleWidget::_totalSuccess = 0;
size_t ConsoleWidget::_totalError = 0;

}  // namespace shkyera
