#pragma once

#include <vector>

#include "Types.hpp"

namespace treeHelpers
{

class RectNodeABC;

/**
 * @brief Class representing a tree structure of nodes.
 *
 * Tree structure of nodes with helper functions such as adding/removing and finding
 * tree nodes.
 */
class TreeStruct
{
public:
    TreeStruct();
    ~TreeStruct();
    void append(RectNodeABC* node);
    void remove(const RectNodeABC* node);
    void find(const RectNodeABC* node);
    void findDeep(const RectNodeABC* node);
    void setParent(RectNodeABC* node);

    bool isRootNode() const;

    treeNodeId getId() const;
    treeNodeLevel getLevel() const;
    const std::vector<RectNodeABC*> getChildren() const;

    void setLevel(const treeNodeLevel level);

private:
    static uint32_t generateId()
    {
        static uint32_t currentId = 1; // Start giving IDs from 1
        return currentId++;
    }

    //TODO: Make sure we cannot parent to ourselves and also make sure that
    // if the node is already parented, we cannot parent it to another node again
    treeNodeId gId;
    treeNodeLevel gLevel{ 1 };
    RectNodeABC* gParent{ nullptr };
    std::vector<RectNodeABC*> gChildren{};
};

}