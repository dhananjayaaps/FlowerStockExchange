#ifndef BACKEND_ALLTREES_H
#define BACKEND_ALLTREES_H

#include "RedBlackTree.h"
#include "order.h"

#define TREE_ARRAY_SIZE  5

namespace sdds{
    static RedBlackTree SellingDataTree[TREE_ARRAY_SIZE];
    static RedBlackTree BuyingDataTree[TREE_ARRAY_SIZE];

    void pushDummiestToTree();

}

#endif //BACKEND_ALLTREES_H
