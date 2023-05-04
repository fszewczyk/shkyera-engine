#include "imgui.h"
#include <imgui_internal.h>

#include <iostream>

#include "core/Image.hpp"
#include "ui/UI.hpp"
#include "ui/widgets/ConsoleWidget.hpp"
#include "ui/widgets/FilesystemWidget.hpp"

namespace shkyera {

void FilesystemWidget::draw() {
    ImGui::Begin(_name.c_str());

    if (_currentDirectory == nullptr) {
        ImGui::Text("No directory specified.");
    } else {
        ImGui::BeginChild("Directories", ImVec2(180, 0));
        drawDirectoryTree(_projectDirectory);
        ImGui::EndChild();

        ImGui::SameLine();

        /// Apologies to the reader for this hack
        ImGui::BeginChild("Border", ImVec2(1, 0), true);
        ImGui::EndChild();

        ImGui::SameLine();

        ImGui::BeginChild("Contents", ImVec2(0, 0), false, ImGuiWindowFlags_NoScrollWithMouse);

        drawDirectoryContents(_currentDirectory);
        handleRightMouseClick();

        ImGui::EndChild();
    }

    ImGui::End();
}

void FilesystemWidget::setDirectory(std::filesystem::path path) {
    _projectDirectory = std::make_shared<Directory>(path);
    _currentDirectory = _projectDirectory;
}

void FilesystemWidget::drawDirectoryTree(const std::shared_ptr<Directory> directory) {
    const std::vector<std::shared_ptr<Directory>> subDirectories = directory->getSubDirectories();

    static bool initiallyOpenedTree = false;

    ImGui::Image((ImTextureID)Image::ICON_FILES_FOLDER.getTextureId(), ImVec2(16, 16));
    ImGui::SameLine();

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow;
    if (subDirectories.empty())
        flags |= ImGuiTreeNodeFlags_Leaf;

    if (!initiallyOpenedTree)
        ImGui::SetNextItemOpen(initiallyOpenedTree == false);

    if (ImGui::TreeNodeEx((directory->getName()).c_str(), flags)) {
        if (ImGui::IsItemClicked())
            _currentDirectory = directory;

        for (const auto &subDir : subDirectories) {
            drawDirectoryTree(subDir);
        }

        ImGui::TreePop();
    }

    initiallyOpenedTree = true;
}

void FilesystemWidget::drawDirectoryContents(const std::shared_ptr<Directory> directory) {
    ImGui::PushFont(UI::BIG_FONT);
    ImGui::Text(directory->getPath().string().c_str());
    ImGui::PopFont();
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0, 3));

    _hoveredIcon = false;

    float totalWidth = ImGui::GetWindowContentRegionMax()[0];

    int iconsToFit = std::max(1, (int)(totalWidth / (CONTENTS_ICON_SIZE + 15)));
    int iconsDrawn = 0;

    const std::vector<std::shared_ptr<Directory>> subDirectories = directory->getSubDirectories();
    const std::vector<std::shared_ptr<File>> files = directory->getFiles();

    for (const auto &subDir : subDirectories) {
        if (iconsDrawn % iconsToFit != 0)
            ImGui::SameLine();

        drawDirectory(subDir);
        iconsDrawn += 1;
    }

    for (const auto &file : files) {
        if (iconsDrawn % iconsToFit != 0)
            ImGui::SameLine();

        drawFile(file);
        iconsDrawn += 1;
    }
}

void FilesystemWidget::drawDirectory(const std::shared_ptr<Directory> directory) {
    ImGui::BeginGroup();
    ImGui::PushID(directory->getName().c_str());

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    if (ImGui::ImageButton((ImTextureID)Image::ICON_FILES_FOLDER.getTextureId(),
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
    if (ImGui::ImageButton(getTextureOfFile(file), ImVec2(CONTENTS_ICON_SIZE, CONTENTS_ICON_SIZE))) {
    }

    _hoveredIcon |= ImGui::IsItemHovered();

    ImGui::PopStyleColor();

    std::string nameToDisplay = getDisplayableName(file->getName());
    drawIconName(nameToDisplay);

    ImGui::PopID();
    ImGui::EndGroup();
}

void FilesystemWidget::drawIconName(const std::string name) const {
    ImGui::PushFont(UI::SMALL_FONT);

    auto textWidth = ImGui::CalcTextSize(name.c_str()).x;
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (CONTENTS_ICON_SIZE + 10 - textWidth) * 0.5f);

    ImGui::Text(name.c_str());

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
                    std::copy(DEFAULT_FILE_NAME.begin(), DEFAULT_FILE_NAME.end(), fileName);
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
                    std::copy(DEFAULT_FOLDER_NAME.begin(), DEFAULT_FOLDER_NAME.end(), folderName);
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

void FilesystemWidget::drawCreateMenu() const { ImGui::OpenPopup("New"); }

ImTextureID FilesystemWidget::getTextureOfFile(const std::shared_ptr<File> file) const {
    switch (file->getType()) {
    case PYTHON:
        return (ImTextureID)Image::ICON_FILES_PYTHON.getTextureId();
        break;
    case IMAGE:
        return (ImTextureID)Image::ICON_FILES_IMAGE.getTextureId();
        break;
    default:
        return (ImTextureID)Image::ICON_FILES_TEXT.getTextureId();
        break;
    }
}

std::string FilesystemWidget::getDisplayableName(std::string name, size_t maxCharactersInLine) {
    for (size_t pos = maxCharactersInLine; pos < name.length(); pos += maxCharactersInLine + 1) {
        name.insert(pos, "\n");
    }

    return name;
}

std::string FilesystemWidget::DEFAULT_FOLDER_NAME = "New Folder";
std::string FilesystemWidget::DEFAULT_FILE_NAME = "object.py";

} // namespace shkyera
