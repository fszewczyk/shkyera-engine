#include <iostream>

#include "imgui.h"
#include <imgui_internal.h>

#include "ui/widgets/ConsoleWidget.hpp"

namespace shkyera {

void LogVerbose::draw() {
    ImGui::Text("Verbose");
    ImGui::SameLine();
    ImGui::Text(_name.c_str());
}

void LogInfo::draw() {
    ImGui::Text("Info");
    ImGui::SameLine();
    ImGui::Text(_name.c_str());
}

void LogSuccess::draw() {
    ImGui::Text("Success");
    ImGui::SameLine();
    ImGui::Text(_name.c_str());
}

void LogError::draw() {
    ImGui::Text("Error");
    ImGui::SameLine();
    ImGui::Text(_name.c_str());
}

void LogPython::draw() {
    ImGui::Text("Python");
    ImGui::SameLine();
    ImGui::Text(_name.c_str());
}

void ConsoleWidget::draw() {
    ImGui::Begin(_name.c_str());

    for (const auto &log : _logs) {
        log->draw();
    }

    ImGui::End();
}

void ConsoleWidget::clear() { _logs.clear(); }

void ConsoleWidget::enableVerbose() { _verbose = true; }
void ConsoleWidget::disableVerbose() { _verbose = false; }

void ConsoleWidget::logVerbose(std::string text) {
    if (_verbose) {
        _logs.push_back(std::make_unique<LogVerbose>(text));
    }
}
void ConsoleWidget::logInfo(std::string text) { _logs.push_back(std::make_unique<LogInfo>(text)); }
void ConsoleWidget::logSuccess(std::string text) { _logs.push_back(std::make_unique<LogSuccess>(text)); }
void ConsoleWidget::logError(std::string text) { _logs.push_back(std::make_unique<LogError>(text)); }
void ConsoleWidget::logPython(std::string text) { _logs.push_back(std::make_unique<LogPython>(text)); }

bool ConsoleWidget::_verbose = true;
std::vector<std::unique_ptr<Log>> ConsoleWidget::_logs = {};

} // namespace shkyera
