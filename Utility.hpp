#ifndef UTILITY_HEADER
#define UTILITY_HEADER

#include <String>
#include "BPlusTree.hpp"
#include "structures.hpp"
#include "Database_Storage.hpp"


int getHeight(BPlusTree *tree) {
    unsigned int index = tree->rootNode;

    if (index == 0) {
        return 0;
    }


    treeNodeBlock *cursor = (treeNodeBlock*)tree->blkManager->accessBlock(tree->rootNode);

    // Tree is empty

    int height = 1;

    // Iterate tree until a leaf node is reached
    while (cursor->type != 2) {
        cursor = (treeNodeBlock*)tree->blkManager->accessBlock(cursor->ptrs[0].getBlock());
        height++;
    }

    return height;
}

#endif
