#ifndef BACKEND_BUYHANDLER_H
#define BACKEND_BUYHANDLER_H
#include "AllTrees.h"
#include "order.h"
#include "RedBlackTree.h"
#include <iostream>

namespace sdds{
//    static RedBlackTree BuyingDataTree[5];

    void handleBuy(const Order& newOrder, int instrument, double price);
}


#endif //BACKEND_BUYHANDLER_H
