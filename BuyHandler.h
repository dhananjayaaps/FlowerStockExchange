#ifndef BACKEND_BUYHANDLER_H
#define BACKEND_BUYHANDLER_H
#include "AllTrees.h"
#include "order.h"
#include "RedBlackTree.h"
#include <iostream>

namespace sdds{
    void pushDummies();
    void popDummies();
    void handleBuy(Order& newOrder, int instrument, double price, int quantity, std::ostringstream &os);
    void handleSell(Order& newOrder, int instrument, double price, int quantity, std::ostringstream &os);
}

#endif //BACKEND_BUYHANDLER_H
