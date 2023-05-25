#ifndef BACKEND_HANDLERLOGIC_H
#define BACKEND_HANDLERLOGIC_H
#include "AllTrees.h"
#include "order.h"
#include "RedBlackTree.h"
#include <iostream>

namespace sdds{
//    static RedBlackTree BuyingDataTree[5];
    void pushDummiestToBuy();
    void pushDummiestToSell();
    void handleBuy(Order& newOrder, int instrument, double price, int quantity, std::vector<std::string> &resp);
    void handleSell(Order& newOrder, int instrument, double price, int quantity, std::vector<std::string> &resp);

}

#endif //BACKEND_HANDLERLOGIC_H
