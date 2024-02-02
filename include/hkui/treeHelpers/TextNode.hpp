#pragma once

#include <string>
#include <vector>

#include "ConcreteNode.hpp"
#include "../textHelpers/TextHelper.hpp"
#include "../../hkui/renderHelpers/RenderHelper.hpp"

namespace treeHelpers
{

class TextNode : public RectNodeABC
{
public:
    TextNode(const std::string& vertPath, const std::string& fragPath);

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
     * @brief Set text color.
     *
     * @param color  Text color.
     */
    void setTextColor(const glm::vec4& color);

private:
    void onRenderDone();

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
    std::string gTextVertPath{ "src/assets/shaders/textV.glsl" };
    std::string gTextFragPath{ "src/assets/shaders/textF.glsl" };
    ConcreteNode node; //TODO: This shall be replaced by render batcher

    MouseClickCb gMouseClickCb{ nullptr };
    MouseDropCb gMouseDropCb{ nullptr };

    textHelpers::LoadedFontPtr lfPtr{ nullptr };

    std::string gText;
    bool gTextIsDirty{ false };
    uint32_t gLetterIdx{ 0 };

    bool gCenterAlign{ false };
    std::vector<textHelpers::TextLine> gTextLines;
    float gLongestLine{ 0.0f };

    renderHelpers::RenderHelper& gRenderInstance;
    textHelpers::TextHelper& gTextHelperInstance;
};

}
