#ifndef BACKEND_SELLHANDLER_H
#define BACKEND_SELLHANDLER_H

#include "AllTrees.h"
#include <queue>
#include "order.h"
#include "RedBlackTree.h"
#include <queue>

namespace sdds{
//    static RedBlackTree SellingDataTree[5];

    void handleSell(const Order& newOrder, int instrument, double price);

}

#endif //BACKEND_SELLHANDLER_H
