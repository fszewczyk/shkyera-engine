#pragma once

#include <AssetManager/Asset.hpp>
#include <AssetManager/Mesh.hpp>
#include <AssetManager/Texture.hpp>
#include <AssetManager/Wireframe.hpp>
#include <Common/Types.hpp>
#include <Components/AssetComponents/AssetComponent.hpp>
#include <Components/NameComponent.hpp>
#include <ECS/Registry.hpp>
#include <UI/Common/Style.hpp>
#include <Utils/AssetUtils.hpp>

namespace shkyera {

template <typename AssetType>
class AssetSelector {
   public:
    AssetSelector() = default;
    AssetSelector(const std::string& title)
        : _title(title) {}
    AssetSelector(const std::string& title, Registry* registry, std::optional<AssetHandle> asset)
        : _title(title), _registry(registry), _asset(asset) {}
    virtual ~AssetSelector() = default;

    void setUpdateCallback(std::function<void(AssetHandle file)> callback) {
        _updateCallback = std::move(callback);
    }

    void setClearCallback(std::function<void()> callback) {
        _clearCallback = std::move(callback);
    }

    void draw() {
        ImGui::TextUnformatted(_title.c_str());

        ImGui::SameLine();
        ImGui::SetCursorPosX(128);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(7.0f, 5.0f));
        ImGui::PushStyleColor(ImGuiCol_ChildBg, style::DARK_ACCENT);
        ImGui::PushFont(style::SMALL_FONT);

        const std::string childName = _title + "Child";
        ImGui::BeginChild(childName.c_str(), ImVec2(-60, 24), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

        if (_asset.has_value() && _registry) {
            std::string displayName = "Unknown Name and Path";

            if (_registry->hasComponent<NameComponent>(*_asset)) {
                displayName = _registry->getComponent<NameComponent>(*_asset).getName();
            } else if constexpr (PathConstructible<AssetType>) {
                if (_registry->hasComponent<AssetComponent<AssetType>>(*_asset)) {
                    const auto& assetComponent = _registry->getComponent<AssetComponent<AssetType>>(*_asset);
                    if (const auto* assetLoader = dynamic_cast<utils::assets::PathAssetLoader<AssetType>*>(assetComponent.constructionFunction.get())) {
                        displayName = assetLoader->path;
                    }
                }
            }

            ImGui::TextUnformatted(displayName.c_str());

            ImGui::SameLine();
            drawAsset(_registry, *_asset);
        } else {
            ImGui::BeginDisabled();

            std::string label = "Drag " + _title;
            ImGui::TextUnformatted(label.c_str());
            ImGui::EndDisabled();
        }

        if (ImGui::BeginDragDropTarget()) {
            const auto assetTypeName = std::string(typeid(AssetType).name());
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(("ASSET_" + assetTypeName).c_str())) {
                AssetHandle handle;
                std::memcpy(&handle, payload->Data, sizeof(AssetHandle));
                _asset = handle;

                if (_updateCallback) {
                    _updateCallback(*_asset);
                }
            }
            ImGui::EndDragDropTarget();
        }

        ImGui::EndChild();

        ImGui::PopFont();
        ImGui::PopStyleColor();
        ImGui::PopStyleVar();

        ImGui::SameLine();
        if (ImGui::Button(std::string("Clear##" + childName).c_str(), ImVec2(50, 22))) {
            _asset.reset();
            clearAsset();
            if (_clearCallback) {
                _clearCallback();
            }
        }
    }

   protected:
    virtual void drawAsset(Registry* registry, AssetHandle handle) {}
    virtual void clearAsset() {}

   private:
    std::string _title{};
    Registry* _registry;
    std::optional<AssetHandle> _asset;

    std::function<void(AssetHandle)> _updateCallback;
    std::function<void()> _clearCallback;
};

class TextureAssetSelector : public AssetSelector<Texture> {
   public:
    using AssetSelector<Texture>::AssetSelector;
    ~TextureAssetSelector() {
        clearAsset();
    }

   private:
    void drawAsset(Registry* registry, AssetHandle handle) override {
        if (registry->hasComponent<AssetComponent<Texture>>(handle)) {
            auto& textureAsset = registry->getComponent<AssetComponent<Texture>>(handle);
            _texture = utils::assets::read(textureAsset);
            const auto imageSize = _texture->getSize();
            const auto aspectRatio = imageSize.x / imageSize.y;

            ImGui::Image(_texture->getImguiTextureID(), {aspectRatio * 16, 16});
        }
    }

    void clearAsset() override {
        _texture.reset();
    }

    AssetRef<Texture> _texture{};  //< Keeps the Texture Asset alive
};

}  // namespace shkyera
