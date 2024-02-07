#pragma once

#include <string>
#include <vector>

#include "../../hkui/renderHelpers/RenderHelper.hpp"
#include "../textHelpers/TextHelper.hpp"
#include "ConcreteNode.hpp"

namespace treeHelpers
{

class TextNode : public RectNodeABC
{
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

    enum Direction
    {
        PreCenter,
        Center,
        PostCenter
    };

    TextNode(const std::string& vertPath, const std::string& fragPath);
    TextNode(const std::string& textVertPath,
        const std::string& textFragPath,
        const std::string& baseVertPath,
        const std::string& baseFragPath);
    ~TextNode() = default;

    /**
     * @brief Register an on click listener for this node.
     *
     * @param callback  Function that will be called on click.
     */
    void registerOnClick(const MouseClickCb callback);

    /**
     * @brief Register an on drop listener for this node.
     *
     * @param callback  Function that will be called on drop.
     */
    void registerOnItemsDrop(const MouseDropCb callback);

    /**
     * @brief Set font of current text node.
     *
     * Sets an active font and font scale for this text node. Path supplied
     * must point to a file of TTF type.
     *
     * @param fontPath  Font path.
     * @param fontSize  Size the font will be loaded at.
     *
     * @return True on success, False on load error.
     */
    bool setFont(const std::string& fontPath, const uint32_t fontSize);

    /**
     * @brief Set text of this text node.
     *
     * @param text  Text to be set.
     */
    void setText(const std::string& text);

    /**
     * @brief Align text vertically and horizontally.
     *
     * @param align  Set alignment yes/no.
     */
    void alignTextToCenter(const bool align);

    /**
     * @brief Align text horizontally.
     *
     * @param dir  Alignment direction to be set.
     */
    void alignTextToHorizontal(const Direction dir);

    /**
     * @brief Set text color.
     *
     * @param color  Text color.
     */
    void setTextColor(const glm::vec4& color);

    /**
     * @brief Adds padding to the text.
     *
     * If text is aligned to the left, padding will be put between container start and text start and analogous for the
     * right alignment. If alignment is set to center, this has no effect
     *
     * @param padding  Padding (in pixels) to be set.
     */
    void setTextHorizontalPadding(const int32_t padding);

    meshHelpers::MeshStyle gStyle;

private:
    IMPL_OF_PARENT(onRenderDone);
    IMPL_OF_PARENT(onMouseButton);
    IMPL_OF_PARENT(onItemsDrop);
    IMPL_OF_PARENT(onWindowResize);

    /**
     * @brief Compute lines of supplied internal text.
     *
     * Computes and places lines computed inside gTextLines structure. Currently lines
     * are added when line overflows parent horizontal space.
     */
    void computeLines();

    /**
     * @brief Compute longest line length among lines in gTextLines.
     *
     */
    void computeLongestLine();

    /* Concept for text rendering, experimental */
    ConcreteNode node; // TODO: This shall be replaced by render batcher

    MouseClickCb gMouseClickCb{nullptr};
    MouseDropCb gMouseDropCb{nullptr};

    textHelpers::LoadedFontPtr lfPtr{nullptr};

    std::string gText;
    bool gTextIsDirty{false};
    uint32_t gLetterIdx{0};

    int32_t gHorizontalPadding{0};
    bool gCenterAlign{false};
    Direction gDir{Direction::Center};
    std::vector<textHelpers::TextLine> gTextLines;
    float gLongestLine{0.0f};

    renderHelpers::RenderHelper& gRenderInstance;
    textHelpers::TextHelper& gTextHelperInstance;
};

} // namespace treeHelpers
