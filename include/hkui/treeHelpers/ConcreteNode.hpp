#pragma once

#include "RectNodeABC.hpp"

namespace treeHelpers
{
class ConcreteNode : public RectNodeABC
{
    typedef std::function<void(ConcreteNode& self, int button, int x, int y)> MouseClickWithSelfCb;

public:
    ConcreteNode(const std::string& vertPath, const std::string& fragPath);

    void registerOnClick(const MouseClickCb callback);
    void registerOnClick(const MouseClickWithSelfCb callback);
    void registerOnRelease(const MouseReleaseCb callback);
    void registerOnMouseHover(const MouseHoverCb callback);
    void registerOnMouseEnter(const MouseEnterCb callback);
    void registerOnMouseExit(const MouseExitCb callback);

private:
    IMPL_OF_PARENT(onMouseButton);
    IMPL_OF_PARENT(onMouseHover);
    IMPL_OF_PARENT(onMouseEnter);
    IMPL_OF_PARENT(onMouseExit);

    MouseClickCb gMouseClickCb{ nullptr };
    MouseClickWithSelfCb gMouseClickWithSelfCb{ nullptr };
    MouseReleaseCb gMouseReleaseCb{ nullptr };
    MouseHoverCb gMouseHoverCb{ nullptr };
    MouseEnterCb gMouseEnterCb{ nullptr };
    MouseExitCb gMouseExitCb{ nullptr };

    /* Basic shader */
    std::string gVertPath;
    std::string gFragPath;
};
}