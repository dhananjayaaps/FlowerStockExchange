#include "SellHandler.h"

namespace sdds {


    void handleSell(const Order& newOrder, int instrument, double price) {

        auto tempRoot = BuyingDataTree[instrument].getRoot();
        double buyMaxValue;

        if (tempRoot->data == -1){
            buyMaxValue = 0;
        } else{
            Node* buyMax = BuyingDataTree[instrument].maxValue();
            buyMaxValue = buyMax->data;
            std::cout << "MAX buy is 2:" <<buyMaxValue;
        }

        std::cout << "MAX buy is :" <<buyMaxValue;


        if (buyMaxValue > price){
            std::cout << "There is a exchange" << std::endl;
            return;
        }
        else{
            std::cout << "There is a New" << std::endl;

            std::queue<Order> *pQueue = SellingDataTree[instrument].insert(price);

            pQueue->push(newOrder);

            SellingDataTree[instrument].printTree();

            std::queue<Order> copyQueue = *pQueue;

            std::cout << std::endl;

            std::cout << std::endl;
        }
    }
}
