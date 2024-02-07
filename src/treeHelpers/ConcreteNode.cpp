#include "../../include/hkui/treeHelpers/ConcreteNode.hpp"

namespace treeHelpers
{
ConcreteNode::ConcreteNode(const std::string& vertPath, const std::string& fragPath)
    : RectNodeABC(vertPath, fragPath)
    , gVertPath(vertPath)
    , gFragPath(fragPath)
{}

void ConcreteNode::onMouseHover()
{
    if (gMouseHoverCb && gStatePtr)
    {
        printf("Hovering concrete\n");
        gMouseHoverCb(gStatePtr->mouseX, gStatePtr->mouseY);
    }
}

void ConcreteNode::onMouseEnter()
{
    if (gMouseEnterCb && gStatePtr) { gMouseEnterCb(gStatePtr->mouseX, gStatePtr->mouseY); }
}

void ConcreteNode::onMouseExit()
{
    if (gMouseExitCb && gStatePtr) { gMouseExitCb(gStatePtr->mouseX, gStatePtr->mouseY); }
}

void ConcreteNode::onMouseButton()
{
    /* If gMouseClickCb and gMouseClickWithSelfCb are both set, they will both fire.
       Just remmeber that. */
    if (gMouseClickCb && gStatePtr && gStatePtr->mouseClicked)
    {
        gMouseClickCb(gStatePtr->button, gStatePtr->mouseX, gStatePtr->mouseY);
    }

    if (gMouseClickWithSelfCb && gStatePtr && gStatePtr->mouseClicked)
    {
        gMouseClickWithSelfCb(*this, gStatePtr->button, gStatePtr->mouseX, gStatePtr->mouseY);
    }

    if (gMouseReleaseCb && gStatePtr && !gStatePtr->mouseClicked)
    {
        gMouseReleaseCb(gStatePtr->button, gStatePtr->mouseX, gStatePtr->mouseY);
    }
}

void ConcreteNode::registerOnClick(const MouseClickCb callback)
{
    gMouseClickCb = callback;
}

void ConcreteNode::registerOnClick(const MouseClickWithSelfCb callback)
{
    gMouseClickWithSelfCb = callback;
}

void ConcreteNode::registerOnRelease(const MouseReleaseCb callback)
{
    gMouseReleaseCb = callback;
}

void ConcreteNode::registerOnMouseHover(const MouseHoverCb callback)
{
    gMouseHoverCb = callback;
}

void ConcreteNode::registerOnMouseExit(const MouseExitCb callback)
{
    gMouseExitCb = callback;
}

void ConcreteNode::registerOnMouseEnter(const MouseEnterCb callback)
{
    gMouseEnterCb = callback;
}

} // namespace treeHelpers