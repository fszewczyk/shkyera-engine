/**
 * @file FilesystemWidget.hpp
 *
 * @brief Contains the declaration of the `FilesystemWidget` class, a user interface widget for interacting with the
 * filesystem.
 */

#pragma once

#include <functional>

#include <Components/NameComponent.hpp>
#include <AssetManager/Texture.hpp>
#include <UI/Widget.hpp>
#include <Utils/AssetUtils.hpp>
#include <ECS/Registry.hpp>

namespace shkyera {

/**
 * @brief A user interface widget for interacting with the filesystem.
 *
 * The `FilesystemWidget` class provides a graphical user interface for navigating and interacting with the filesystem,
 * displaying directories, files, and enabling file-related actions.
 */
class FilesystemWidget : public Widget {
  public:
    FilesystemWidget(std::string name, std::shared_ptr<Registry> registry, AssetHandle rootDirectoryHandle);

    /**
     * @brief Implementation of the abstract `draw` method to render the filesystem widget.
     */
    virtual void draw() override;

  private:
    /**
     * @brief Draw the directory tree, starting from the specified directory.
     *
     * @param directory The root directory to draw.
     */
    void drawDirectoryTree(AssetHandle directoryHandle);

    /**
     * @brief Draw the contents of the specified directory.
     *
     * @param directory The directory whose contents to draw.
     */
    void drawDirectoryContents();

    /**
     * @brief Draw a directory icon and name.
     *
     * @param directoryHandle Handle of the directory to draw.
     */
    void drawDirectory(AssetHandle directoryHandle);

    /**
     * @brief Draw an asset icon and name.
     *
     * @param assetHandle Handle of the asset to draw.
     * @param icon Icon of the asset
     */
    template<typename AssetType>
    void drawAsset(AssetHandle handle)
    {
      const auto& fileName = _registry->getComponent<NameComponent>(handle).getName();

      ImGui::BeginGroup();
      ImGui::PushID(fileName.c_str());

      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

      drawAssetIcon<AssetType>(handle);

      const auto assetTypeName = std::string(typeid(AssetType).name());
      if(ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
      {
        ImGui::TextUnformatted(fileName.c_str());
        ImGui::SetDragDropPayload(("ASSET_" + assetTypeName).c_str(), reinterpret_cast<void *>(&handle), sizeof(AssetHandle));
        ImGui::EndDragDropSource();
      }
      _hoveredIcon |= ImGui::IsItemHovered();

      ImGui::PopStyleColor();

      drawIconName(fileName);

      ImGui::PopID();
      ImGui::EndGroup();
    }

    /**
     * Draws the icon of an asset and allows for its drag-and-drop behavior.
     * @tparam Type of the asset
     * @param hande Asset Handle of the drawn asset
     */
    template<typename AssetType>
    void drawAssetIcon(AssetHandle handle);

    /**
     * @brief Draw an icon and name within the UI.
     *
     * @param name The name to display.
     */
    void drawIconName(const std::string& name) const;

    void handleRightMouseClick();

    static constexpr float CONTENTS_ICON_SIZE = 64; ///< The size of icons representing directory contents.

    std::shared_ptr<Registry> _registry;
    AssetHandle _rootDirectoryHandle;
    AssetHandle _currentDirectoryHandle;
    AssetRef<Texture> _folderIcon;
    AssetRef<Texture> _pythonIcon;
    AssetRef<Texture> _imageIcon;
    AssetRef<Texture> _textIcon;
    bool _hoveredIcon;
};

} // namespace shkyera
