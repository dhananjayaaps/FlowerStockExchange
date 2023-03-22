#ifndef BACKEND_ALLTREES_H
#define BACKEND_ALLTREES_H

#include "RedBlackTree.h"
#include "order.h"

namespace sdds{
    const int TREE_ARRAY_SIZE = 5;

    static RedBlackTree SellingDataTree[TREE_ARRAY_SIZE];
    static RedBlackTree BuyingDataTree[TREE_ARRAY_SIZE];
}

#endif //BACKEND_ALLTREES_H
