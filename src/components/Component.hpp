#pragma once

#include <string>

namespace shkyera {

class Component {
  public:
    virtual void process() = 0;
};

} // namespace shkyera
