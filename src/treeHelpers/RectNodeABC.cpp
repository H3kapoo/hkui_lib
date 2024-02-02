#include "../../include/hkui/treeHelpers/RectNodeABC.hpp"

#include <stdio.h>

#include "../../include/hkui/utils/CommonUtils.hpp"

namespace treeHelpers
{

RectNodeABC::RectNodeABC()
    : RectNodeABC(baseV, baseF, true)
{}

RectNodeABC::RectNodeABC(const std::string& vertCorP, const std::string& fragCorP,
    const bool immediate)
    : gShInstance{ shaderHelpers::ShaderHelper::get() }
    , gMesh{ vertCorP, fragCorP, immediate }
{}

RectNodeABC::~RectNodeABC()
{
    if (gFastTreeSortPtr)
    {
        delete gFastTreeSortPtr;
    }
}

void RectNodeABC::enableFastTreeSort()
{
    if (!gFastTreeSortPtr)
    {
        gFastTreeSortPtr = new FastTreeSort();
        printf("FastTreeSort enabled for node with id: %d\n", gTreeStruct.getId());
    }
}

void RectNodeABC::updateFastTree()
{
    if (gFastTreeSortPtr)
    {
        gFastTreeSortPtr->sortBuffer(this, [](const RectNodeABC* x, const RectNodeABC* y) -> bool
            {
                /* Sort so that vec is ordered by highest level first */
                return x->gTreeStruct.getLevel() > y->gTreeStruct.getLevel();
            });
        printf("FastTree updated!\n");
        return;
    }

    fprintf(stderr, "FastTreeSort is not enabled!\n");
}

void RectNodeABC::setEventTransparent(const bool value)
{
    gIsEventTransparent = value;
}

void RectNodeABC::emitEvent(const inputHelpers::Event& evt)
{
    /* If root node */
    if (gTreeStruct.getId() == 1)
    {
        utils::logAndExitOnNull(gFastTreeSortPtr, "Fast tree was not initialized!");
    }

    switch (evt)
    {
    case inputHelpers::Event::WindowResize:
        for (const auto& c : gFastTreeSortPtr->getBuffer())
        {
            c->onWindowResize();
        }
        break;
    case inputHelpers::Event::MouseButton:
        searchForMouseSelection();
        break;
    case inputHelpers::Event::MouseMove:
        searchForMouseHover();
        onMouseHover();
        break;
    case inputHelpers::Event::RenderDone:
        onRenderDone();
        break;
    case inputHelpers::Event::ItemsDrop:
        searchForMouseDropLoc();
        break;
    default:
        fprintf(stderr, "Unknown base event: %d\n", static_cast<int>(evt));
        break;
    }
}

void RectNodeABC::append(RectNodeABC* child)
{
    /* Z axis shall not be user modified and it's soley used with the depth buffer to
       prevent overdraw. */
    child->gMesh.gBox.pos.z = gTreeStruct.getLevel() + 1;
    child->gTreeStruct.setParent(this);
    child->gStatePtr = gStatePtr;

    gTreeStruct.append(child);
}

void RectNodeABC::setStateSource(stateHelpers::WindowState* state)
{
    gStatePtr = state;
}

void RectNodeABC::searchForMouseSelection()
{
    if (!gStatePtr)
    {
        fprintf(stderr, "Window State pointer is not set for node with id: %d\n", gTreeStruct.getId()); exit(1);
    }

    if (gTreeStruct.isRootNode() && gFastTreeSortPtr != nullptr)
    {
        int32_t x = 0;
        for (const auto& c : gFastTreeSortPtr->getBuffer())
        {
            if (c->gIsEventTransparent) { continue; }

            int mouseX = gStatePtr->mouseX;
            int mouseY = gStatePtr->mouseY;
            auto& mesh = c->gMesh;
            if ((mouseX > mesh.gBox.pos.x) && (mouseX < mesh.gBox.pos.x + mesh.gBox.scale.x) &&
                (mouseY > mesh.gBox.pos.y) && (mouseY < mesh.gBox.pos.y + mesh.gBox.scale.y))
            {
                gStatePtr->selectedId = c->gTreeStruct.getId();
                c->onMouseButton();
                break;
            }
            x++;
        }
    }
}

void RectNodeABC::searchForMouseHover()
{
    if (!gStatePtr)
    {
        fprintf(stderr, "Window State pointer is not set for node with id: %d\n", gTreeStruct.getId()); exit(1);
    }

    if (gTreeStruct.isRootNode() && gFastTreeSortPtr != nullptr)
    {
        bool changeNeeded{ false };
        for (const auto& c : gFastTreeSortPtr->getBuffer())
        {
            if (c->gIsEventTransparent) { continue; }

            int mouseX = gStatePtr->mouseX;
            int mouseY = gStatePtr->mouseY;
            auto& mesh = c->gMesh;
            if ((mouseX > mesh.gBox.pos.x) && (mouseX < mesh.gBox.pos.x + mesh.gBox.scale.x) &&
                (mouseY > mesh.gBox.pos.y) && (mouseY < mesh.gBox.pos.y + mesh.gBox.scale.y))
            {
                auto newHoveredId = c->gTreeStruct.getId();
                if (newHoveredId != gStatePtr->hoveredId)
                {
                    gStatePtr->prevHoveredId = gStatePtr->hoveredId;
                    changeNeeded = true;
                }

                gStatePtr->hoveredId = newHoveredId;
                c->onMouseHover();
                break;
            }
        }

        bool notifiedExit{ false }, notifiedEnter{ false };
        for (const auto& c : gFastTreeSortPtr->getBuffer())
        {
            if (!changeNeeded || (notifiedEnter && notifiedExit))
            {
                break;
            }

            auto id = c->gTreeStruct.getId();
            if (id == gStatePtr->hoveredId)
            {
                c->onMouseEnter();
                notifiedEnter = true;
            }

            if (id == gStatePtr->prevHoveredId)
            {
                c->onMouseExit();
                notifiedExit = true;
            }
        }
    }
}

void RectNodeABC::searchForMouseDropLoc()
{
    if (!gStatePtr)
    {
        fprintf(stderr, "Window State pointer is not set for node with id: %d\n", gTreeStruct.getId()); exit(1);
    }

    if (gTreeStruct.isRootNode() && gFastTreeSortPtr != nullptr)
    {
        for (const auto& c : gFastTreeSortPtr->getBuffer())
        {
            if (gIsEventTransparent) { continue; }

            int mouseX = gStatePtr->mouseX;
            int mouseY = gStatePtr->mouseY;
            auto& mesh = c->gMesh;
            if ((mouseX > mesh.gBox.pos.x) && (mouseX < mesh.gBox.pos.x + mesh.gBox.scale.x) &&
                (mouseY > mesh.gBox.pos.y) && (mouseY < mesh.gBox.pos.y + mesh.gBox.scale.y))
            {
                c->onItemsDrop();
                break;
            }
        }
    }
}
}