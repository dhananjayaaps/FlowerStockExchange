#include "BuyHandler.h"


namespace sdds {

    void handleBuy(const Order& newOrder, int instrument, double price) {
        BuyingDataTree[instrument].insert(price);
        BuyingDataTree[instrument].printTree();
        std::cout<< std::endl;
    }
}