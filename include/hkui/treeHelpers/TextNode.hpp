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

    void registerOnClick(const MouseClickCb callback);
    void registerOnItemsDrop(const MouseDropCb callback);

    void setText(const std::string& text);
    void alignTextToCenter(const bool align);
    void setTextColor(const glm::vec4& color);

private:
    void onRenderDone();

    IMPL_OF_PARENT(onMouseButton);
    IMPL_OF_PARENT(onItemsDrop);
    IMPL_OF_PARENT(onWindowResize);

    void computeLines();
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
