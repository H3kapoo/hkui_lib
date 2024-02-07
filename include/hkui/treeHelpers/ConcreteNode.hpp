#pragma once

#include "RectNodeABC.hpp"

namespace treeHelpers
{
class ConcreteNode : public RectNodeABC
{
    typedef std::function<void(ConcreteNode& self, int button, int x, int y)> MouseClickWithSelfCb;

public:
    /**
     * @brief Simple class retaining details about the style of the mesh
     *
     */
    struct MeshStyle
    {
        glm::vec4 gGradColor1{0.0f, 0.0f, 0.0f, 1.0f};
        glm::vec4 gGradColor2{0.0f, 0.0f, 0.0f, 1.0f};
        glm::vec4 gBorderColor{0.0f, 0.0f, 0.0f, 1.0f};
        glm::vec4 gBorderSize{0.0f}; /* Top Bot Left Right order */
        uint32_t gTextureId{0};
    };

    ConcreteNode(const std::string& vertPath, const std::string& fragPath);
    ~ConcreteNode() = default;

    void registerOnClick(const MouseClickCb callback);
    void registerOnClick(const MouseClickWithSelfCb callback);
    void registerOnRelease(const MouseReleaseCb callback);
    void registerOnMouseHover(const MouseHoverCb callback);
    void registerOnMouseEnter(const MouseEnterCb callback);
    void registerOnMouseExit(const MouseExitCb callback);

    meshHelpers::MeshStyle gStyle;

private:
    IMPL_OF_PARENT(onMouseButton);
    IMPL_OF_PARENT(onMouseHover);
    IMPL_OF_PARENT(onMouseEnter);
    IMPL_OF_PARENT(onMouseExit);

    MouseClickCb gMouseClickCb{nullptr};
    MouseClickWithSelfCb gMouseClickWithSelfCb{nullptr};
    MouseReleaseCb gMouseReleaseCb{nullptr};
    MouseHoverCb gMouseHoverCb{nullptr};
    MouseEnterCb gMouseEnterCb{nullptr};
    MouseExitCb gMouseExitCb{nullptr};

    /* Basic shader */
    std::string gVertPath;
    std::string gFragPath;
};
} // namespace treeHelpers