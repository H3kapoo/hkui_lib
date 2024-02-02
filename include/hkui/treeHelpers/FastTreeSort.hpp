#pragma once

#include <vector>
#include <functional>
#include <algorithm>

namespace treeHelpers
{
class RectNodeABC;
/**
 * @brief Class used to speed-up searches in the default TreeStruct.
 *
 * Speeds up tree searching by flattening and sorting the tree into an
 * array according to the supplied function inside **sortBuffer**. Internal flat vector
 * needs to be manually asked to sort itself, it doesn't do that autonomously to allow
 * user to also sort tree in batches.
 * It is recommended to ask for resorting after each single/bached node addition/removal
 * in order to avoid UB.
 *
 * @note This should really only be instantiated on the **Root** node of the tree.
 */
class FastTreeSort
{
public:
    /**
     * @brief Get a reference to the internal fast tree buffer.
     *
     * @note This doesn't call **sortBuffer** internally, make sure you already called
     *       it beforehand if necessary.
     *
     * @return Vector reference to the buffer.
     */
    std::vector<RectNodeABC*>& getBuffer();

    /**
     * @brief Populate and sort the internal buffer based on the tree starting at the provided **root** node
     *        and supplied **sortFunc**.
     *
     * @param root      Node to build buffer from.
     * @param sortFunc  Function used to sort the internal buffer.
     */
    void sortBuffer(RectNodeABC* root,
        const std::function<bool(const RectNodeABC* x, const RectNodeABC* y)> sortFunc);

private:
    /**
     * @brief Internal - push a reference to all the nodes inside the internal buffer.
     *
     * @param node Node to be analyzed.
     */
    void recPushChildren(RectNodeABC* node);

    std::vector<RectNodeABC*> gBuffer{};
};

}