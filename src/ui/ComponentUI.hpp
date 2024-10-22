#pragma once

namespace shkyera {

class ComponentUI {
    public:
        virtual ~ComponentUI() = default;

        virtual void draw() = 0;
};

}