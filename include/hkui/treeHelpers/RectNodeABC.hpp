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
    /* Basic inline shader for rectNode */
    const std::string baseV = R"(
#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec2 vTex;

uniform mat4 uModelMatrix;
uniform mat4 uProjMatrix;

out vec2 texOut;

void main()
{
    texOut = vTex;
    gl_Position = uProjMatrix * uModelMatrix * vec4(vPos.xyz, 1.0);
}
)";

    const std::string baseF = R"(
#version 330 core

uniform vec3 uResolution;
uniform vec4 uInnerColor;
uniform vec4 uBorderColor;
uniform vec4 uBorderSize;
uniform float uTime;

in vec2 texOut;

void main()
{
    float ar = uResolution.x / uResolution.y;
    vec2 uv = texOut.xy;;
    uv.x *= ar;

    float oneOverX = 1.0f / (uResolution.x * ar);
    float oneOverY = 1.0f / uResolution.y;
    float borderTop = uBorderSize.x * oneOverY;
    float borderBot = uBorderSize.y * oneOverY;
    float borderLeft = uBorderSize.z * oneOverX;
    float borderRight = uBorderSize.w * oneOverX;

    vec2 dbl = step(vec2(borderLeft, borderTop), uv.xy);
    float v1 = min(dbl.x, dbl.y);

    vec2 dtr = step(vec2(borderRight, borderBot), vec2(1.0*ar-uv.x, 1.0-uv.y));
    float v2 = min(dtr.x, dtr.y);

    vec4 borderItself = vec4(min(v1, v2));
    
    vec4 color = mix(uBorderColor, uInnerColor, borderItself);

    gl_FragColor = color;
}
)";

public:
    RectNodeABC();
    RectNodeABC(const std::string& vertCorP, const std::string& fragCorP, const bool immediate = false);
    virtual ~RectNodeABC();

    /**
     * @brief Enable fast tree sort option for the *root* object.
     *
     * Enabling this option on the root makes it possible to have fast searching
     * or selected/hovered over/etc nodes. It is MANDATORY for now.
     */
    void enableFastTreeSort();

    /**
     * @brief Update internal state of fast tree node.
     *
     * Function required to be called in order to keep the fast tree up to date
     * after each insertion/deletion of a node (or batch of nodes).
     */
    void updateFastTree();

    /**
     * @brief Set node to be transparent to any events that might occur on it.
     *
     * Set node to be transparent to any events that might occur on it like hover,
     * click, etc. The next valid node after this one will receive the event instead.
     *
     * @note Currently this makes the node transparent to ALL events. In the future it will
     *       be better to make it transparent only to certain events.
     *
     * @param value Boolean deciding if node should be transparent or not.
     */
    void setEventTransparent(const bool value);

    /**
     * @brief Emit event throughout the tree structure's nodes.
     *
     * Emit events down the tree structure to notify interested nodes
     * of such events like mouse button actions, mouse moves, keyboard pressed, etc.
     *
     * @param evt Event to be emmited.
     */
    void emitEvent(const inputHelpers::Event& evt);

    /**
     * @brief Append new children to this node.
     *
     * Appends new children to current node by setting appropriate depth level,
     * Z coords, parent, window state and calling the append method on the internal **Tree Structure**.
     *
     * @param child - Node to be added as a child.
     */
    void append(RectNodeABC* child);

    /**
     * @brief Sets the unique set of states for the node.
     *
     * Each window has a global window state that each node shall have access to read
     * and populate according to events that take place throughout the tree structure.
     *
     * @param state State to share.
     */
    void setStateSource(stateHelpers::WindowState* state);


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
    /**
     * @brief Interanl - just find out who will be the selected node upon mouse being
     *        clicked and trigger the event on the concrete class of the node.
     */
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