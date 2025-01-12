#include <iostream>

#include <imgui_internal.h>
#include "imgui.h"

#include <Components/NameComponent.hpp>
#include <Components/TransformComponent.hpp>
#include <UI/Widgets/ConsoleWidget.hpp>
#include <UI/Widgets/ObjectsWidget.hpp>
#include <UI/Widgets/PropertiesWidget.hpp>

namespace shkyera {

ObjectsWidget::ObjectsWidget(std::string name) : Widget(name) {}

void ObjectsWidget::setRegistry(std::shared_ptr<Registry> registry) {
  _registry = registry;
}

void ObjectsWidget::draw() {
  ImGui::Begin(_name.c_str());
  drawCreate();
  ImGui::Separator();
  const auto& hierarchy = _registry->getHierarchy();
  for (const auto& [entity, nameComponent] : _registry->getComponentSet<NameComponent>()) {
    if(!hierarchy.getParent(entity))
    {
      drawObjectHierarchy(entity, hierarchy, 0);
    }
  }
  ImGui::End();
}

void ObjectsWidget::drawCreate() {
  if (ImGui::Button("Create"))
    ImGui::OpenPopup("Create Popup");
  if (ImGui::BeginPopup("Create Popup")) {
    if (ImGui::Selectable("Empty Object")) {
      const auto newEntity = _registry->addEntity();
      _registry->addComponent<NameComponent>(newEntity);
      _registry->addComponent<TransformComponent>(newEntity);

      for (const auto& callback : _onNewEntityCallbacks) {
        callback(newEntity);
      }

      ConsoleWidget::logSuccess("Created Empty Object");
    }

    ImGui::EndPopup();
  }
}

void ObjectsWidget::drawObjectHierarchy(Entity parent, const EntityHierarchy& hierarchy, size_t depth) {
  constexpr size_t MaximumDepth = 6;
  if(depth > MaximumDepth)
  {
    ImGui::Text("** Maximum Hierarchy Depth Reached **");
    return;
  }

  ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow;
  const auto& children = hierarchy.getChildren(parent);
  if(children.empty()) {
    flags |= ImGuiTreeNodeFlags_Leaf;
  }
  
  static bool initiallyOpenedTree = false;
  if (!initiallyOpenedTree)
    ImGui::SetNextItemOpen(initiallyOpenedTree == false);

  const auto& name = _registry->getComponent<NameComponent>(parent).getName();
  const auto& uniqueName = name + "##" + std::to_string(parent);
  if (ImGui::TreeNodeEx(uniqueName.c_str(), flags)) {
    if (ImGui::BeginDragDropTarget()) {
        if (ImGui::AcceptDragDropPayload("DRAG_AND_DROP_ENTITY")) {
          _registry->getHierarchy().attributeChild(parent, _draggedEntity);
        }

        ImGui::EndDragDropTarget();
    }

    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
      ImGui::SameLine();
      ImGui::TextUnformatted(name.c_str());
      _draggedEntity = parent;
      ImGui::SetDragDropPayload("DRAG_AND_DROP_ENTITY", nullptr, 0);
      ImGui::EndDragDropSource();
    }

    if (ImGui::IsItemClicked() && !_registry->getSelectedEntities().contains(parent)) {
      _registry->clearSelectedEntities();
      _registry->selectEntity(parent);
    }

    for (const auto& child : children) {
      drawObjectHierarchy(child, hierarchy, depth + 1);
    }

    ImGui::TreePop();
  }

  initiallyOpenedTree = true;
}

}  // namespace shkyera
