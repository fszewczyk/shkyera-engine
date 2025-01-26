#include <imgui_internal.h>
#include "imgui.h"

#include <iostream>
#include <algorithm>

#include <AssetManager/Texture.hpp>
#include <AssetManager/Shader.hpp>
#include <AssetManager/Mesh.hpp>
#include <Components/AssetComponents/DirectoryComponent.hpp>
#include <UI/Common/Style.hpp>
#include <UI/UI.hpp>
#include <UI/Widgets/ConsoleWidget.hpp>
#include <UI/Widgets/FilesystemWidget.hpp>

namespace shkyera {

FilesystemWidget::FilesystemWidget(std::string name, std::shared_ptr<Registry> registry, AssetHandle rootDirectoryHandle) : Widget(name), _registry(registry), _rootDirectoryHandle(rootDirectoryHandle), _currentDirectoryHandle(rootDirectoryHandle)
{
  _folderIcon = utils::assets::readPermanent<Texture>(Image::ICON_FILES_FOLDER);
  _pythonIcon = utils::assets::readPermanent<Texture>(Image::ICON_FILES_PYTHON);
  _imageIcon = utils::assets::readPermanent<Texture>(Image::ICON_FILES_IMAGE);
  _textIcon = utils::assets::readPermanent<Texture>(Image::ICON_FILES_TEXT);
}

void FilesystemWidget::draw() {
  ImGui::Begin(_name.c_str());

  if (!_registry->hasComponent<DirectoryComponent>(_rootDirectoryHandle)) {
    ImGui::TextUnformatted("Root project directory is not registered as an Asset.");
  } else {
    ImGui::BeginChild("Directories", ImVec2(180, 0));
    drawDirectoryTree(_rootDirectoryHandle);
    ImGui::EndChild();

    ImGui::SameLine();

    /// Apologies to the reader for this hack
    ImGui::BeginChild("Border", ImVec2(1, 0), true);
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("Contents", ImVec2(0, 0), false);

    drawDirectoryContents();

    ImGui::EndChild();
  }

  ImGui::End();
}

void FilesystemWidget::drawDirectoryTree(AssetHandle directoryHandle) {
  std::vector<AssetHandle> subDirectories = utils::assets::getSubdirectories(directoryHandle, _registry.get());

  static bool initiallyOpenedTree = false;
  ImGui::Image(_folderIcon->getImguiTextureID(), ImVec2(16, 16));
  ImGui::SameLine();

  ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow;
  if (subDirectories.empty())
  {
    flags |= ImGuiTreeNodeFlags_Leaf;
  }

  if (!initiallyOpenedTree)
  {
    ImGui::SetNextItemOpen(initiallyOpenedTree == false);
  }

  const auto directoryComponent = _registry->getComponent<DirectoryComponent>(directoryHandle);
  if (ImGui::TreeNodeEx(utils::assets::getName(directoryComponent).c_str(), flags)) 
  {
    if (ImGui::IsItemClicked())
    {
      _currentDirectoryHandle = directoryHandle;
    }

    for (const auto& subDir : subDirectories)
    {
      if(_registry->hasComponent<DirectoryComponent>(subDir))
      {
        drawDirectoryTree(subDir);
      }
    }

    ImGui::TreePop();
  }

  initiallyOpenedTree = true;
}

void FilesystemWidget::drawDirectoryContents() {
  const auto directoryComponent = _registry->getComponent<DirectoryComponent>(_currentDirectoryHandle);

  ImGui::PushFont(style::BIG_FONT);
  ImGui::TextUnformatted(utils::assets::getName(directoryComponent).c_str());
  ImGui::PopFont();
  ImGui::Separator();
  ImGui::Dummy(ImVec2(0, 3));

  _hoveredIcon = false;

  float totalWidth = ImGui::GetWindowContentRegionMax()[0];

  int iconsToFit = std::max(1, (int)(totalWidth / (CONTENTS_ICON_SIZE + 15) - 1));
  int iconsDrawn = 0;

  for (const auto& subDir : utils::assets::getSubdirectories(_currentDirectoryHandle, _registry.get())) {
    if (iconsDrawn % iconsToFit != 0)
    {
      ImGui::SameLine();
    }

    drawDirectory(subDir);
    iconsDrawn++;
  }

  for(const auto& file : _registry->getHierarchy().getChildren(_currentDirectoryHandle))
  {
    if(!_registry->hasComponent<NameComponent>(file))
    {
      return;
    }
    if (iconsDrawn % iconsToFit != 0)
    {
      ImGui::SameLine();
    }

    bool drawn = false;
    if(_registry->hasComponent<AssetComponent<Texture>>(file))
    {
      drawn = true;
      drawAsset<Texture>(file);
    }
    else if(_registry->hasComponent<AssetComponent<Mesh>>(file))
    {
      drawn = true;
      drawAsset<Mesh>(file);
    }
    else if(_registry->hasComponent<AssetComponent<Shader>>(file))
    {
      drawn = true;
      drawAsset<Shader>(file);
    }

    if(drawn)
    {
      iconsDrawn++;
    }
  }
}

void FilesystemWidget::drawDirectory(AssetHandle directoryHandle) {
  const auto& directoryName = _registry->getComponent<NameComponent>(directoryHandle).getName();

  ImGui::BeginGroup();
  ImGui::PushID(directoryName.c_str());

  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
  if (ImGui::ImageButton(_folderIcon->getImguiTextureID(),
                         ImVec2(CONTENTS_ICON_SIZE, CONTENTS_ICON_SIZE))) {
    _currentDirectoryHandle = directoryHandle;
  }

  _hoveredIcon |= ImGui::IsItemHovered();

  ImGui::PopStyleColor();

  drawIconName(directoryName);

  ImGui::PopID();
  ImGui::EndGroup();
}

template<>
void FilesystemWidget::drawAssetIcon<Texture>(AssetHandle handle)
{
  if(ImGui::ImageButton(_imageIcon->getImguiTextureID(), ImVec2(CONTENTS_ICON_SIZE, CONTENTS_ICON_SIZE))) 
  {

  }
}

template<>
void FilesystemWidget::drawAssetIcon<Mesh>(AssetHandle handle)
{
  if(ImGui::ImageButton(_textIcon->getImguiTextureID(), ImVec2(CONTENTS_ICON_SIZE, CONTENTS_ICON_SIZE))) 
  {
                              
  }
}

template<>
void FilesystemWidget::drawAssetIcon<Shader>(AssetHandle handle)
{
  if(ImGui::ImageButton(_textIcon->getImguiTextureID(), ImVec2(CONTENTS_ICON_SIZE, CONTENTS_ICON_SIZE)))
  {

  }
}

void FilesystemWidget::drawIconName(const std::string& name) const {
  ImGui::PushFont(style::SMALL_FONT);

  constexpr int MAX_CHAR_PER_LINE = 15;
  std::vector<std::string> lines;
  for (size_t i = 0; i < name.length(); i += MAX_CHAR_PER_LINE) 
  {
    lines.push_back(name.substr(i, MAX_CHAR_PER_LINE));
  }

  float totalHeight = lines.size() * ImGui::GetTextLineHeightWithSpacing();
  float iconCenterOffset = (CONTENTS_ICON_SIZE + 10 - totalHeight) * 0.5f;

  for (const auto& line : lines) 
  {
    auto textWidth = ImGui::CalcTextSize(line.c_str()).x;
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() +
                         (CONTENTS_ICON_SIZE + 10 - textWidth) * 0.5f);
    ImGui::TextUnformatted(line.c_str());
  }

  ImGui::PopFont();
}

}  // namespace shkyera
