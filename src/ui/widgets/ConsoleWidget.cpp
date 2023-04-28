#include <iostream>

#include "imgui.h"
#include <imgui_internal.h>

#include "core/Image.hpp"
#include "ui/widgets/ConsoleWidget.hpp"

namespace shkyera {

Log::Log(std::string content) : _content(content) {}

void Log::draw() const {
    ImGui::Image((ImTextureID)getIconId(), ImVec2(24, 24));
    ImGui::SameLine();
    ImGui::Text(_content.c_str());
    ImGui::Separator();
}

uint64_t LogVerbose::getIconId() const { return Image::ICON_CONSOLE_VERBOSE.getTextureId(); }
uint64_t LogInfo::getIconId() const { return Image::ICON_CONSOLE_INFO.getTextureId(); }
uint64_t LogSuccess::getIconId() const { return Image::ICON_CONSOLE_SUCCESS.getTextureId(); }
uint64_t LogError::getIconId() const { return Image::ICON_CONSOLE_ERROR.getTextureId(); }

void ConsoleWidget::draw() {
    ImGui::Begin(_name.c_str());

    drawInfoBar();

    ImGui::Separator();

    for (const auto &log : _logs) {
        log->draw();
    }

    ImGui::End();
}

void ConsoleWidget::drawInfoBar() const {
    if (ImGui::Button("Clear"))
        clear();

    ImGui::SameLine();
    ImGui::Image((ImTextureID)Image::ICON_CONSOLE_TOTAL.getTextureId(), ImVec2(22, 22));
    ImGui::SameLine();
    ImGui::Text((std::to_string(_logs.size()) + " Total").c_str());
    ImGui::SameLine();
    ImGui::Image((ImTextureID)Image::ICON_CONSOLE_ERROR.getTextureId(), ImVec2(22, 22));
    ImGui::SameLine();
    ImGui::Text((std::to_string(_totalError) + " Error").c_str());
    ImGui::SameLine();
    ImGui::Image((ImTextureID)Image::ICON_CONSOLE_SUCCESS.getTextureId(), ImVec2(22, 22));
    ImGui::SameLine();
    ImGui::Text((std::to_string(_totalSuccess) + " Success").c_str());
    ImGui::SameLine();
    ImGui::Image((ImTextureID)Image::ICON_CONSOLE_INFO.getTextureId(), ImVec2(22, 22));
    ImGui::SameLine();
    ImGui::Text((std::to_string(_totalInfo) + " Info").c_str());
    ImGui::SameLine();
    ImGui::Image((ImTextureID)Image::ICON_CONSOLE_VERBOSE.getTextureId(), ImVec2(22, 22));
    ImGui::SameLine();
    ImGui::Text((std::to_string(_totalVerbose) + " Verbose").c_str());
}

void ConsoleWidget::clear() {
    _logs.clear();

    _totalVerbose = 0;
    _totalSuccess = 0;
    _totalInfo = 0;
    _totalError = 0;
}

void ConsoleWidget::enableVerbose() { _verbose = true; }
void ConsoleWidget::disableVerbose() { _verbose = false; }

void ConsoleWidget::logVerbose(std::string text) {
    if (_verbose) {
        _logs.push_back(std::make_unique<LogVerbose>(text));
        _totalVerbose++;
    }
}
void ConsoleWidget::logInfo(std::string text) {
    _logs.push_back(std::make_unique<LogInfo>(text));
    _totalInfo++;
}
void ConsoleWidget::logSuccess(std::string text) {
    _logs.push_back(std::make_unique<LogSuccess>(text));
    _totalSuccess++;
}
void ConsoleWidget::logError(std::string text) {
    _logs.push_back(std::make_unique<LogError>(text));
    _totalError++;
}

bool ConsoleWidget::_verbose = true;
std::vector<std::unique_ptr<Log>> ConsoleWidget::_logs = {};

size_t ConsoleWidget::_totalVerbose = 0;
size_t ConsoleWidget::_totalInfo = 0;
size_t ConsoleWidget::_totalSuccess = 0;
size_t ConsoleWidget::_totalError = 0;

} // namespace shkyera
