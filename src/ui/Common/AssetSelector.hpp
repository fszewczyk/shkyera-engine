#pragma once

#include <AssetManager/Mesh.hpp>
#include <AssetManager/Wireframe.hpp>
#include <Common/Types.hpp>
#include <Components/NameComponent.hpp>
#include <Components/AssetComponents/AssetComponent.hpp>
#include <ECS/Registry.hpp>
#include <UI/Common/Style.hpp>

namespace shkyera {

template<typename AssetType>
class AssetSelector {
    public:
        AssetSelector(const std::string& title, std::shared_ptr<Registry> registry, std::optional<AssetHandle> asset) : _title(title), _registry(registry), _asset(asset) {}

        void setUpdateCallback(std::function<void(AssetHandle file)> callback)
        {
            _updateCallback = std::move(callback);
        }

        void setClearCallback(std::function<void()> callback)
        {
            _clearCallback = std::move(callback);
        }

        void draw()
        {
            ImGui::TextUnformatted(_title.c_str());

            ImGui::SameLine();
            ImGui::SetCursorPosX(85);

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(7.0f, 5.0f));
            ImGui::PushStyleColor(ImGuiCol_ChildBg, style::DARK_ACCENT);

            const std::string childName = _title + "Child";
            ImGui::BeginChild(childName.c_str(), ImVec2(-60, 24), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

            if (_asset.has_value()) 
            {
                std::string displayName = "Unknown Name and Path";

                if(_registry->hasComponent<NameComponent>(*_asset))
                {
                    displayName = _registry->getComponent<NameComponent>(*_asset).getName();
                }
                else if constexpr(std::is_base_of_v<PathConstructibleAsset<AssetType>, AssetType>)
                {
                    if(_registry->hasComponent<AssetComponent<AssetType>>(*_asset))
                    {
                        const auto& assetComponent = _registry->getComponent<AssetComponent<AssetType>>(*_asset);
                        if(const auto assetPtr = assetComponent.shaderPtr.lock())
                        {
                            if(const auto assetPath = assetPtr->getPath())
                            {
                                displayName = *assetPath;
                            }
                        }
                    }
                }

                ImGui::TextUnformatted(displayName.c_str());
            } 
            else
            {
                ImGui::BeginDisabled();

                std::string label = "Drag " + _title;
                ImGui::TextUnformatted(label.c_str());
                ImGui::EndDisabled();
            }

            if (ImGui::BeginDragDropTarget()) {
                const auto assetTypeName = std::string(typeid(AssetType).name());
                if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload(("ASSET_" + assetTypeName).c_str())) {
                    AssetHandle handle;
                    std::memcpy(&handle, payload->Data, sizeof(AssetHandle));
                    _asset = handle;

                    if(_updateCallback) {
                        _updateCallback(*_asset);
                    }
                }
                ImGui::EndDragDropTarget();
            }


            ImGui::EndChild();

            ImGui::PopStyleColor();
            ImGui::PopStyleVar();

            ImGui::SameLine();
            if(ImGui::Button("Clear", ImVec2(50, 22))) {
                _asset = std::nullopt;
                if(_clearCallback) {
                    _clearCallback();
                }
            }
        }

    private:
        std::string _title;
        std::shared_ptr<Registry> _registry;
        std::optional<AssetHandle> _asset;

        std::function<void(AssetHandle)> _updateCallback;
        std::function<void()> _clearCallback;
};

}
