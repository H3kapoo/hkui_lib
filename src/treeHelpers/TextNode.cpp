#include "../../include/hkui/treeHelpers/TextNode.hpp"

#include <algorithm>
#include <utility>

#include "../../include/hkui/utils/CommonUtils.hpp"

namespace treeHelpers
{

TextNode::TextNode(const std::string& vertPath, const std::string& fragPath)
    : RectNodeABC()
    , node{vertPath, fragPath}
    , gRenderInstance{renderHelpers::RenderHelper::get()}
    , gTextHelperInstance{textHelpers::TextHelper::get()}
{
    /* Watch out shader's uniforms for changes. Set default if any not used. */
    gMesh.gUniKeeper.watch("uInnerColor", &gMesh.gColor);
    gMesh.gUniKeeper.watch("uBorderColor", &gStyle.gBorderColor);
    gMesh.gUniKeeper.watch("uBorderSize", &gStyle.gBorderSize);
    gMesh.gUniKeeper.watch("uResolution", &gMesh.gBox.scale);

    node.gMesh.gUniKeeper.watch("uColor", &node.gMesh.gColor);
    node.gMesh.gUniKeeper.watch("uCharIndex", &gLetterIdx);

    node.gMesh.gColor = utils::hexToVec4("#f1f1f1");

    node.gMesh.gBox.scale.x = 32;
    node.gMesh.gBox.scale.y = 32;
}

TextNode::TextNode(const std::string& textVertPath,
    const std::string& textFragPath,
    const std::string& baseVertPath,
    const std::string& baseFragPath)
    : RectNodeABC(baseVertPath, baseFragPath)
    , node{textVertPath, textFragPath}
    , gRenderInstance{renderHelpers::RenderHelper::get()}
    , gTextHelperInstance{textHelpers::TextHelper::get()}
{
    node.gMesh.gUniKeeper.watch("uColor", &node.gMesh.gColor);
    node.gMesh.gUniKeeper.watch("uCharIndex", &gLetterIdx);

    node.gMesh.gColor = utils::hexToVec4("#f1f1f1");

    node.gMesh.gBox.scale.x = 32;
    node.gMesh.gBox.scale.y = 32;
}

bool TextNode::setFont(const std::string& fontPath, const uint32_t fontSize)
{
    lfPtr = gTextHelperInstance.loadFont(fontPath, fontSize);
    if (!lfPtr) return false;

    /* Texture is not hotreloadable for now */
    node.gMesh.gUniKeeper.gTexturePtrs.clear();
    node.gMesh.gUniKeeper.gTexturePtrs.push_back(std::make_pair("uTextureArrayId", lfPtr->id));
    return true;
}

void TextNode::setText(const std::string& text)
{
    gText = text;

    /* This needs to be done outside of the constructor and after THIS node gets parented
       otherwise Z will be incorrect. */
    const float textDepth = gTreeStruct.getLevel() + 0.5f; // slightly push it upwards
    node.gMesh.gBox.pos.z = textDepth;

    gTextIsDirty = true;
}

void TextNode::alignTextToCenter(const bool align)
{
    gCenterAlign = align;
}

void TextNode::alignTextToHorizontal(const Direction dir)
{
    gDir = dir;
}

void TextNode::setTextColor(const glm::vec4& color)
{
    node.gMesh.gColor = color;
}

void TextNode::setTextHorizontalPadding(const int32_t padding)
{
    gHorizontalPadding = padding;
}

void TextNode::computeLines()
{
    utils::logAndExitOnNull(lfPtr, "No font has been loaded!");

    gTextLines.clear();

    const float maxX = gMesh.gBox.scale.x;

    float currentLength = 0;
    int32_t maxHeight = 0;
    uint32_t startIdx = 0;
    uint32_t endIdx = 0;
    for (uint32_t i = 0; i < gText.length(); i++)
    {
        textHelpers::ASCIIChar& chData = lfPtr->data[gText[i]];

        float nextLength = currentLength + (chData.hAdvance >> 6);
        if (nextLength > maxX)
        {
            endIdx = i;
            gTextLines.emplace_back(startIdx, endIdx, currentLength, maxHeight);
            startIdx = i;
            maxHeight = 0;
            currentLength = (chData.hAdvance >> 6);
        }
        else
        {
            maxHeight = std::max(maxHeight, chData.size.y);
            currentLength = nextLength;
        }
    }

    /* Put the line at the end that doesnt fill the parent entirely */
    if (endIdx < gText.length()) { gTextLines.emplace_back(endIdx, gText.length(), currentLength, maxHeight); }
}

void TextNode::onRenderDone()
{
    if (gTextIsDirty)
    {
        computeLines();
        gTextIsDirty = false;
    }
    // TODO: Implement text batching
    // TODO: This shall be recalculated only on text/font change since text wont change its size magically.

    glDepthMask(GL_FALSE);

    float heights = 0;
    for (const auto& line : gTextLines)
    {
        heights += line.height;
    }
    // printf("Heights %f\n", heights);

    int32_t lineNo = 1;
    for (const auto& line : gTextLines)
    {
        float bringDown = 32 * lineNo++;
        float x;

        switch (gDir)
        {
        case Direction::PreCenter:
            x = gMesh.gBox.pos.x + gHorizontalPadding;
            break;
        case Direction::Center:
            x = gMesh.gBox.pos.x + (gMesh.gBox.scale.x - line.length) * 0.5f;
            break;
        case Direction::PostCenter:
            x = gMesh.gBox.pos.x + (gMesh.gBox.scale.x - line.length) - gHorizontalPadding;
            break;
        }

        // x = gMesh.gBox.pos.x + (gMesh.gBox.scale.x - line.length) * 0.5f * gCenterAlign;
        float y = gMesh.gBox.pos.y + bringDown + (gMesh.gBox.scale.y - heights) * 0.5f;
        for (uint32_t i = line.startIdx; i < line.endIdx; i++)
        {
            textHelpers::ASCIIChar& chData = lfPtr->data[gText[i]];

            gLetterIdx = chData.charCode;

            float xPos = x + chData.bearing.x;
            float yPos = y - chData.bearing.y;
            /* Cast needed because text rendering with float positioning makes text blurry sometimes */
            node.gMesh.gBox.pos.x = (int32_t)xPos;
            node.gMesh.gBox.pos.y = (int32_t)yPos;
            gRenderInstance.renderRectNode(node);

            x += (chData.hAdvance >> 6);
        }
    }
    glDepthMask(GL_TRUE);
}

void TextNode::onWindowResize()
{
    gTextIsDirty = true;
}

void TextNode::onMouseButton()
{
    if (gMouseClickCb && gStatePtr && gStatePtr->mouseClicked)
    {
        gMouseClickCb(gStatePtr->button, gStatePtr->mouseX, gStatePtr->mouseY);
    }
}

void TextNode::onItemsDrop()
{
    if (gMouseDropCb && gStatePtr) { gMouseDropCb(gStatePtr->dropCount, gStatePtr->droppedPaths); }
}

void TextNode::registerOnClick(const MouseClickCb callback)
{
    gMouseClickCb = callback;
}

void TextNode::registerOnItemsDrop(const MouseDropCb callback)
{
    gMouseDropCb = callback;
}
} // namespace treeHelpers