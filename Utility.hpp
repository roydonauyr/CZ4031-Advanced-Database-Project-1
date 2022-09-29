#ifndef UTILITY_HEADER
#define UTILITY_HEADER

#include <String>
#include <BPlusTree.hpp>
#include <structures.hpp>
#include <Database_Storage.hpp>

class Utility {
private:
    BPlusTree *bPlusTree;
public:
    int getHeight(BPlusTree *tree) {
        unsigned int index = tree->rootNode;
        treeNodeBlock *cursor = static_cast<treeNodeBlock*>(tree->blkManager->accessBlock(tree->rootNode));

        // Tree is empty
        if (cursor == NULL) {
            return 0;
        }

        int height = 1;

        // Iterate tree until a leaf node is reached
        while (cursor->type != 2) {
            cursor = static_cast<treeNodeBlock*>(tree->blkManager->accessBlock(cursor->ptrs[0].getBlock()));
            height++;
        }

        return height;
    }
};

#endif
