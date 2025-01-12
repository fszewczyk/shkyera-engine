#include <imgui_internal.h>
#include "imgui.h"

#include <iostream>

#include <AssetManager/AssetManager.hpp>
#include <AssetManager/Image.hpp>
#include <UI/Common/Style.hpp>
#include <UI/UI.hpp>
#include <UI/Widgets/ConsoleWidget.hpp>
#include <UI/Widgets/FilesystemWidget.hpp>

namespace shkyera {

FilesystemWidget::FilesystemWidget(std::string name) : Widget(name)
{
  _folderIcon = AssetManager::getInstance().getAsset<Texture>(Image::ICON_FILES_FOLDER);
  _pythonIcon = AssetManager::getInstance().getAsset<Texture>(Image::ICON_FILES_PYTHON);
  _imageIcon = AssetManager::getInstance().getAsset<Texture>(Image::ICON_FILES_IMAGE);
  _textIcon = AssetManager::getInstance().getAsset<Texture>(Image::ICON_FILES_TEXT);
}

void FilesystemWidget::draw() {
  ImGui::Begin(_name.c_str());

  if (_currentDirectory == nullptr) {
    ImGui::TextUnformatted("No directory specified.");
  } else {
    ImGui::BeginChild("Directories", ImVec2(180, 0));
    drawDirectoryTree(Directory::getRoot());
    ImGui::EndChild();

    ImGui::SameLine();

    /// Apologies to the reader for this hack
    ImGui::BeginChild("Border", ImVec2(1, 0), true);
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("Contents", ImVec2(0, 0), false,
                      ImGuiWindowFlags_NoScrollWithMouse);

    drawDirectoryContents(_currentDirectory);
    handleRightMouseClick();

    ImGui::EndChild();
  }

  ImGui::End();
}

void FilesystemWidget::setDirectory(std::filesystem::path path) {
  _currentDirectory = std::make_shared<Directory>(path);
  Directory::setRoot(_currentDirectory);
}

void FilesystemWidget::drawDirectoryTree(
    const std::shared_ptr<Directory> directory) {
  const std::vector<std::shared_ptr<Directory>> subDirectories =
      directory->getSubDirectories();

  static bool initiallyOpenedTree = false;

  ImGui::Image(_folderIcon->getImguiTextureID(),
               ImVec2(16, 16));
  ImGui::SameLine();

  ImGuiTreeNodeFlags flags =
      ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow;
  if (subDirectories.empty())
    flags |= ImGuiTreeNodeFlags_Leaf;

  if (!initiallyOpenedTree)
    ImGui::SetNextItemOpen(initiallyOpenedTree == false);

  if (ImGui::TreeNodeEx((directory->getName()).c_str(), flags)) {
    if (ImGui::IsItemClicked())
      _currentDirectory = directory;

    for (const auto& subDir : subDirectories) {
      drawDirectoryTree(subDir);
    }

    ImGui::TreePop();
  }

  initiallyOpenedTree = true;
}

void FilesystemWidget::drawDirectoryContents(
    const std::shared_ptr<Directory> directory) {
  ImGui::PushFont(style::BIG_FONT);
  ImGui::TextUnformatted(directory->getPath().string().c_str());
  ImGui::PopFont();
  ImGui::Separator();
  ImGui::Dummy(ImVec2(0, 3));

  _hoveredIcon = false;

  float totalWidth = ImGui::GetWindowContentRegionMax()[0];

  int iconsToFit = std::max(1, (int)(totalWidth / (CONTENTS_ICON_SIZE + 15)));
  int iconsDrawn = 0;

  const std::vector<std::shared_ptr<Directory>> subDirectories =
      directory->getSubDirectories();
  const std::vector<std::shared_ptr<File>> files = directory->getFiles();

  for (const auto& subDir : subDirectories) {
    if (iconsDrawn % iconsToFit != 0)
      ImGui::SameLine();

    drawDirectory(subDir);
    iconsDrawn += 1;
  }

  for (const auto& file : files) {
    if (iconsDrawn % iconsToFit != 0)
      ImGui::SameLine();

    drawFile(file);
    iconsDrawn += 1;
  }
}

void FilesystemWidget::drawDirectory(
    const std::shared_ptr<Directory> directory) {
  ImGui::BeginGroup();
  ImGui::PushID(directory->getName().c_str());

  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
  if (ImGui::ImageButton(_folderIcon->getImguiTextureID(),
                         ImVec2(CONTENTS_ICON_SIZE, CONTENTS_ICON_SIZE))) {
    _currentDirectory = directory;
  }

  _hoveredIcon |= ImGui::IsItemHovered();

  ImGui::PopStyleColor();

  std::string nameToDisplay = getDisplayableName(directory->getName());
  drawIconName(nameToDisplay);

  ImGui::PopID();
  ImGui::EndGroup();
}

void FilesystemWidget::drawFile(const std::shared_ptr<File> file) {
  ImGui::BeginGroup();
  ImGui::PushID(file->getName().c_str());

  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
  if (ImGui::ImageButton(getTextureOfFile(file),
                         ImVec2(CONTENTS_ICON_SIZE, CONTENTS_ICON_SIZE))) {}

  if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
    std::string fileName = file->getName();
    std::string filePath = file->getPath().string();

    ImGui::TextUnformatted(filePath.c_str());
    ImGui::SetDragDropPayload("DRAG_AND_DROP_FILE", filePath.c_str(),
                              filePath.length() * sizeof(char));

    ImGui::EndDragDropSource();
  }

  _hoveredIcon |= ImGui::IsItemHovered();

  ImGui::PopStyleColor();

  std::string nameToDisplay = getDisplayableName(file->getName());
  drawIconName(nameToDisplay);

  ImGui::PopID();
  ImGui::EndGroup();
}

void FilesystemWidget::drawIconName(const std::string name) const {
  ImGui::PushFont(style::SMALL_FONT);

  auto textWidth = ImGui::CalcTextSize(name.c_str()).x;
  ImGui::SetCursorPosX(ImGui::GetCursorPosX() +
                       (CONTENTS_ICON_SIZE + 10 - textWidth) * 0.5f);

  ImGui::TextUnformatted(name.c_str());

  ImGui::PopFont();
}

void FilesystemWidget::handleRightMouseClick() {
  if (!_hoveredIcon && ImGui::IsWindowHovered() && ImGui::IsMouseClicked(1)) {
    drawCreateMenu();
  }

  if (ImGui::BeginPopup("New")) {
    if (ImGui::BeginMenu("New File")) {
      static char fileName[64] = "object.py";
      ImGui::InputText("##", fileName, 64);

      ImGui::SameLine();

      if (ImGui::Button("Create")) {
        try {
          _currentDirectory->createFile(std::string(fileName));

          memset(fileName, 0, sizeof(fileName));
          std::copy(DEFAULT_FILE_NAME.begin(), DEFAULT_FILE_NAME.end(),
                    fileName);
          ImGui::CloseCurrentPopup();
        } catch (std::invalid_argument error) {
          ConsoleWidget::logError(std::string(error.what()));

          ImGui::CloseCurrentPopup();
          ImGui::SetWindowFocus("Console");
        }
      }

      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("New Folder")) {
      static char folderName[64] = "New Folder";
      ImGui::InputText("##", folderName, 64);

      ImGui::SameLine();

      if (ImGui::Button("Create")) {
        try {
          _currentDirectory->createDirectory(std::string(folderName));

          memset(folderName, 0, sizeof(folderName));
          std::copy(DEFAULT_FOLDER_NAME.begin(), DEFAULT_FOLDER_NAME.end(),
                    folderName);
          ImGui::CloseCurrentPopup();
        } catch (std::invalid_argument error) {
          ConsoleWidget::logError(std::string(error.what()));

          ImGui::CloseCurrentPopup();
          ImGui::SetWindowFocus("Console");
        }
      }

      ImGui::EndMenu();
    }

    ImGui::EndPopup();
  }
}

void FilesystemWidget::drawCreateMenu() const {
  ImGui::OpenPopup("New");
}

ImTextureID FilesystemWidget::getTextureOfFile(
    const std::shared_ptr<File> file) const {
  switch (file->getType()) {
    case PYTHON:
      return _pythonIcon->getImguiTextureID();
      break;
    case IMAGE:
      return _imageIcon->getImguiTextureID();
      break;
    default:
      return _textIcon->getImguiTextureID();
      break;
  }
}

std::string FilesystemWidget::getDisplayableName(std::string name,
                                                 size_t maxCharactersInLine) {
  for (size_t pos = maxCharactersInLine; pos < name.length();
       pos += maxCharactersInLine + 1) {
    name.insert(pos, "\n");
  }

  return name;
}

std::string FilesystemWidget::DEFAULT_FOLDER_NAME = "New Folder";
std::string FilesystemWidget::DEFAULT_FILE_NAME = "object.py";

}  // namespace shkyera
