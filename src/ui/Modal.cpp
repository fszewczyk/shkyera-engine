#include "imgui.h"

#include "ui/Modal.hpp"

namespace shkyera {

template <typename T> Modal<T>::Modal(std::string title, std::string message) : _title(title), _message(message) {}

template <typename T> T Modal<T>::draw() {
    if (ImGui::BeginPopupModal(_title.c_str)) {
        ImGui::EndPopup();
    }
}

} // namespace shkyera
