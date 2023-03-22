#include "BuyHandler.h"
#include <ostream>
#include <sstream>
#include <limits>

namespace sdds {

    void pushDummiestToSell() {
        for (auto & i : SellingDataTree) {
            i.insert(std::numeric_limits<double>::infinity());
        }
    }

    void handleSell(Order &newOrder, int instrument, double price, int quantity) {
        std::ostringstream os;
        std::cout << "-Sell-" << std::endl;

        //Get the maximum Buying Order
        Node* buyMax = BuyingDataTree[instrument].maxValue();
        std::cout <<"Maximum is "<<buyMax->data <<std::endl;

        //If it lower than the Selling order Price, It add to the tree without executing
        if(buyMax->data < price) {

            std::queue<Order> *pQueue = SellingDataTree[instrument].insert(price);
            pQueue->push(newOrder);
            newOrder.newOrd(os);
            os<<std::endl;
        } else{
            std::cout << "handling" <<std::endl;
            //for handle
            while (quantity != 0){
                int availQuantity = buyMax->orderQueue.front().getQuan();
                if(availQuantity == quantity){
                    newOrder.fill(os);
                    os<<std::endl;
                    buyMax->orderQueue.front().fill(os);
                    buyMax->orderQueue.pop();
                    if(buyMax->orderQueue.empty()){
                        SellingDataTree[instrument].deleteNode(buyMax->data);
                    }
                    break;
                }
                while (availQuantity < quantity){
                    newOrder.pfill(os, quantity);
                    os<<std::endl;
                    buyMax->orderQueue.front().fill(os);
                    buyMax->orderQueue.pop();
                    if(buyMax->orderQueue.empty()){
                        SellingDataTree[instrument].deleteNode(buyMax->data);
                        buyMax = BuyingDataTree[instrument].maxValue();
                        if (buyMax->data < price){

                        }
                    }
                    availQuantity = buyMax->orderQueue.front().getQuan();
                }

            }

        }
        std::cout << os.str();
        std::cout<< std::endl;

    }

    void pushDummiestToBuy() {
        for (auto & i : BuyingDataTree) {
            i.insert(-1.0);
        }
    }
    void handleBuy(Order& newOrder, int instrument, double price, int quantity) {
        std::ostringstream os;
        std::cout << "-Buy-" << std::endl;
        //Get the minimum selling Order
        Node* SellMin = SellingDataTree[instrument].minValue();
        std::cout <<"Minimum is "<<SellMin->data;

        //If it higher than the Buyer order Price, It add to the tree without executing
        if(SellMin->data < price) {
            std::queue<Order> *pQueue = BuyingDataTree[instrument].insert(price);
            pQueue->push(newOrder);
        } else{
            std::cout << "handling";
            //for handle
        }
        std::cout<< std::endl;
    }
}