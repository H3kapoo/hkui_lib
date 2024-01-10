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
    std::vector<RectNodeABC*>& getBuffer();
    void sortBuffer(RectNodeABC* root,
        const std::function<bool(const RectNodeABC* x, const RectNodeABC* y)> sortFunc);

private:
    void recPushChildren(RectNodeABC* node);

    std::vector<RectNodeABC*> gBuffer{};
};

}