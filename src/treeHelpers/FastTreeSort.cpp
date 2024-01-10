#include "../../include/hkui/treeHelpers/FastTreeSort.hpp"

#include "../../include/hkui/treeHelpers/RectNodeABC.hpp"

namespace treeHelpers
{

/**
 * @brief Populate and sort the internal buffer based on the tree starting at the provided **root** node
 *        and supplied **sortFunc**.
 *
 * @param root      Node to build buffer from.
 * @param sortFunc  Function used to sort the internal buffer.
 */
void FastTreeSort::sortBuffer(RectNodeABC* root,
    const std::function<bool(const RectNodeABC* x, const RectNodeABC* y)> sortFunc)
{
    gBuffer.clear();
    recPushChildren(root);

    std::sort(gBuffer.begin(), gBuffer.end(), sortFunc);
}


/**
 * @brief Internal - push a reference to all the nodes inside the internal buffer.
 *
 * @param node Node to be analyzed.
 */
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


/**
 * @brief Get a reference to the internal fast tree buffer.
 *
 * @note This doesn't call **sortBuffer** internally, make sure you already called
 *       it beforehand if necessary.
 *
 * @return Vector reference to the buffer.
 */
std::vector<RectNodeABC*>& FastTreeSort::getBuffer()
{
    return gBuffer;
}

}