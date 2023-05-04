#pragma once

#include <string>

namespace shkyera {

template <typename T> class Modal {
    Modal(std::string title, std::string message);

    T draw();

  private:
    std::string _title;
    std::string _message;
};

} // namespace shkyera
