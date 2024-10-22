#pragma once

#include <memory>
#include <AssetManager/Wireframe.hpp>
#include <Components/BaseComponent.hpp>

namespace shkyera {

class WireframeComponent : public BaseComponent<WireframeComponent> {
public:
    WireframeComponent() = default;

    void setWireframe(std::shared_ptr<Wireframe> wireframe) {
        _wireframe = wireframe;
    }

    Wireframe const* getWireframe() const {
        return _wireframe.get();
    }

    void updateImpl() const {
        if (_wireframe) {
            _wireframe->bind();
            glDrawArrays(GL_LINES, 0, _wireframe->getEdgeCount());
            _wireframe->unbind();
        }
    }

private:
    std::shared_ptr<Wireframe> _wireframe;
};

} // namespace shkyera
