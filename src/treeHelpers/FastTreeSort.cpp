#include "../../include/hkui/treeHelpers/FastTreeSort.hpp"

#include "../../include/hkui/treeHelpers/RectNodeABC.hpp"

namespace treeHelpers
{

void FastTreeSort::sortBuffer(RectNodeABC* root,
    const std::function<bool(const RectNodeABC* x, const RectNodeABC* y)> sortFunc)
{
    gBuffer.clear();
    recPushChildren(root);

    std::sort(gBuffer.begin(), gBuffer.end(), sortFunc);
}

void FastTreeSort::recPushChildren(RectNodeABC* node)
{
    if (!node)
    {
        fprintf(stderr, "Null Node while sorting FastTree! This shouldn't happen.\n");
        return;
    }

    gBuffer.push_back(node);
    for (const auto& ch : node->gTreeStruct.getChildren())
    {
        recPushChildren(ch);
    }
}

std::vector<RectNodeABC*>& FastTreeSort::getBuffer()
{
    return gBuffer;
}

}