#include <imgui_internal.h>
#include "imgui.h"

#include <AssetManager/Material.hpp>
#include <AssetManager/Mesh.hpp>
#include <AssetManager/Shader.hpp>
#include <AssetManager/Wireframe.hpp>
#include <Common/Clock.hpp>
#include <Common/InstanceCounter.hpp>
#include <Common/Profiler.hpp>
#include <UI/Common/Style.hpp>
#include <UI/Widgets/ProfilerWidget.hpp>
#include <string_view>
#include <type_traits>

namespace shkyera {

ProfilerWidget::ProfilerWidget(std::string name) : Widget(std::move(name)) {}

void ProfilerWidget::draw() {
  ImGui::Begin(_name.c_str());

  ImGui::PushFont(style::HUGE_FONT);
  ImGui::TextUnformatted("Instance Counters");
  ImGui::PopFont();

  if (ImGui::BeginTable("Instance Counters", 4, ImGuiTableFlags_Resizable | ImGuiTableFlags_Borders)) {
    ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_None, 0.0f, 0);
    ImGui::TableSetupColumn("Instantiated", ImGuiTableColumnFlags_None, 0.0f, 1);
    ImGui::TableSetupColumn("Alive", ImGuiTableColumnFlags_None, 0.0f, 2);
    ImGui::TableSetupColumn("Moved", ImGuiTableColumnFlags_None, 0.0f, 2);
    ImGui::TableSetupScrollFreeze(0, 1);
    ImGui::TableHeadersRow();

    const auto drawConstructionStats = [](const char* label, auto typeTag) {
      using T = typename decltype(typeTag)::type;

      ImGui::TableNextColumn();
      ImGui::TextUnformatted(label);
      ImGui::TableNextColumn();
      ImGui::Text("%zu", InstanceCounter<T>::Instantiated.load());
      ImGui::TableNextColumn();
      ImGui::Text("%zu", InstanceCounter<T>::Alive.load());
      ImGui::TableNextColumn();
      ImGui::Text("%zu", InstanceCounter<T>::Moved.load());
    };

    drawConstructionStats("Materials", std::type_identity<Material>{});
    drawConstructionStats("Mesh", std::type_identity<Mesh>{});
    drawConstructionStats("Wireframe", std::type_identity<Wireframe>{});
    drawConstructionStats("Shader", std::type_identity<Shader>{});
    drawConstructionStats("Image", std::type_identity<Image>{});

    ImGui::EndTable();
  }

  ImGui::PushFont(style::HUGE_FONT);
  ImGui::TextUnformatted("Timing profiles");
  ImGui::PopFont();

  size_t threadNumber = 0;
  const auto profileBlocks = SHKYERA_READ_PROFILE;
  const auto currentTime = std::chrono::high_resolution_clock::now();
  const auto timeSinceLastReset = currentTime - mTimeOfLastReset;
  const double timeSinceLastResetNanoseconds =
      static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(timeSinceLastReset).count());

  mFramesSinceLastReset++;
  bool shouldReset = false;
  ImGui::Checkbox("Reset after each frame", &mResetOnEachFrame);
  if (mResetOnEachFrame) {
    shouldReset = true;
  } else {
    ImGui::SameLine();
    if (ImGui::Button("Reset")) {
      shouldReset = true;
    }
  }

  const auto averageNsPerFrame = static_cast<float>(timeSinceLastResetNanoseconds) / mFramesSinceLastReset;
  const auto avergeSecPerFrame = averageNsPerFrame / 1e9;
  const auto averageFramesPerSecond = static_cast<int>(1.0f / avergeSecPerFrame);
  ImGui::TextUnformatted(std::string("Frames Per Second: " + std::to_string(averageFramesPerSecond)).c_str());

  for (const auto& [threadId, blocks] : profileBlocks) {
    if (ImGui::BeginTable("Timing Profiles", 4, ImGuiTableFlags_Resizable | ImGuiTableFlags_Borders)) {
      ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_None, 0.0f, 0);
      ImGui::TableSetupColumn("Mean [ms]", ImGuiTableColumnFlags_None, 0.0f, 1);
      ImGui::TableSetupColumn("Calls", ImGuiTableColumnFlags_None, 0.0f, 2);
      ImGui::TableSetupColumn("\% of Frame", ImGuiTableColumnFlags_None, 0.0f, 3);
      ImGui::TableSetupScrollFreeze(0, 1);
      ImGui::TableHeadersRow();

      ImGui::PushFont(style::BIG_FONT);
      ImGui::TableNextColumn();
      ImGui::TextUnformatted("Full Frame");
      ImGui::TableNextColumn();
      ImGui::Text("%.3f", timeSinceLastResetNanoseconds / 1e6 / mFramesSinceLastReset);
      ImGui::TableNextColumn();
      ImGui::Text("%zu", mFramesSinceLastReset);
      ImGui::TableNextColumn();
      ImGui::TextUnformatted("100");
      ImGui::PopFont();

      for (const auto& [name, block] : blocks) {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::TextUnformatted(name.c_str());
        ImGui::TableNextColumn();
        ImGui::Text("%.3f", block.totalLengthInNanoSeconds / 1e6 / block.numberOfCalls);
        ImGui::TableNextColumn();
        ImGui::Text("%zu", block.numberOfCalls);
        ImGui::TableNextColumn();
        ImGui::Text("%.3f", 100 * block.totalLengthInNanoSeconds / timeSinceLastResetNanoseconds);
      }
      ImGui::EndTable();
    }
  }

  ImGui::End();

  if (shouldReset) {
    reset();
  }
}

void ProfilerWidget::reset() {
  SHKYERA_CLEAR_PROFILE;
  mTimeOfLastReset = std::chrono::high_resolution_clock::now();
  mFramesSinceLastReset = 0;
}

}  // namespace shkyera
