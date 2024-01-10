#include "../../include/hkui/treeHelpers/TreeStruct.hpp"

#include "../../include/hkui/treeHelpers/RectNodeABC.hpp"

namespace treeHelpers
{
TreeStruct::TreeStruct()
    : gId{ generateId() } {}

TreeStruct::~TreeStruct()
{
}

void TreeStruct::append(RectNodeABC* node)
{
    node->gTreeStruct.setLevel(gLevel + 1);
    gChildren.push_back(node);
}

void TreeStruct::remove(const RectNodeABC*)
{

}

void TreeStruct::find(const RectNodeABC*)
{
}

void TreeStruct::findDeep(const RectNodeABC*)
{
}

void TreeStruct::setParent(RectNodeABC* node)
{
    gParent = node;
}

bool TreeStruct::isRootNode() const
{
    return gLevel == 1 && gParent == nullptr;
}

treeNodeId TreeStruct::getId() const
{
    return gId;
}

treeNodeLevel TreeStruct::getLevel() const
{
    return gLevel;
}

const std::vector<RectNodeABC*> TreeStruct::getChildren() const
{
    return gChildren;
}

void TreeStruct::setLevel(const treeNodeLevel level)
{
    gLevel = level;
}

}