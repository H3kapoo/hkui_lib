#pragma once

#include <string>

#include "TreeStruct.hpp"
#include "Types.hpp"
#include "FastTreeSort.hpp"
#include "../../hkui/meshHelpers/RectMesh.hpp"
#include "../../hkui/meshHelpers/MeshStyle.hpp"
#include "../../hkui/inputHelpers/Types.hpp"
#include "../stateHelpers/WindowState.hpp"
#include "../../hkui/shaderHelpers/ShaderHelper.hpp"

namespace treeHelpers
{
#define CHILD_MAY_IMPLEMENT(x) virtual void x () {}
#define CHILD_MUST_IMPLEMENT(x) virtual void x () = 0
#define IMPL_OF_PARENT(x) void x () override

/**
 * @brief Abstract base class representing rectangle node.
 *
 * Class used to be inherited from to create new types of nodes sharing
 * basic event/structure functionality.
 *
 */
class RectNodeABC
{
public:
    RectNodeABC(const std::string& vertPath, const std::string& fragPath);
    virtual ~RectNodeABC();

    void enableFastTreeSort();
    void updateFastTree();

    void setEventTransparent(const bool value);

    void append(RectNodeABC* child);

    void setStateSource(stateHelpers::WindowState* state);

    void emitEvent(const inputHelpers::Event& evt);

    TreeStruct gTreeStruct;

protected:
    CHILD_MAY_IMPLEMENT(onMouseButton);
    CHILD_MAY_IMPLEMENT(onMouseHover);
    CHILD_MAY_IMPLEMENT(onRenderDone);
    CHILD_MAY_IMPLEMENT(onItemsDrop);
    CHILD_MAY_IMPLEMENT(onMouseEnter);
    CHILD_MAY_IMPLEMENT(onMouseExit);
    CHILD_MAY_IMPLEMENT(onWindowResize);

    shaderHelpers::ShaderHelper& gShInstance;
    stateHelpers::WindowState* gStatePtr{ nullptr };
private:
    void searchForMouseSelection();
    void searchForMouseHover();
    void searchForMouseDropLoc();

    FastTreeSort* gFastTreeSortPtr{ nullptr };

    bool gIsEventTransparent{ false };
public:
    /* Basic mesh */
    meshHelpers::RectMesh gMesh;
    meshHelpers::MeshStyle gStyle;
};
}