#include "HandlerLogic.h"
#include <sstream>
#include <limits>
#include <iostream>

namespace sdds {

    // That dummies are avoiding some errors with the red black tree.
    // That avoid the no element in the tree state
    // In that point pushing infinity.

    void pushDummiestToSell() {
        for (auto & i : SellingDataTree) {
            i.insert(std::numeric_limits<double>::infinity());
        }
    }

    // That codes are used for handle Sell requests without any errors

    void handleSell(Order &newOrder, int instrument, double price, int quantity, std::vector<std::string> &resp) {
//        std::resptringstream resp;

        //Get the maximum Buying Order
        Node* buyMax = BuyingDataTree[instrument].maxValue();

        //If it lower than the Selling order Price, It add to the tree without executing
        if(buyMax->data < price) {

            std::queue<Order> *pQueue = SellingDataTree[instrument].insert(price);
            pQueue->push(newOrder);
            newOrder.newOrd(resp);
        } else{

            //for handle
            int StartQuantity = quantity;
            int availQuantity = buyMax->orderQueue.front().getQuan();

            while (true){

                // getting the maximum buying request

                if(availQuantity == quantity){
                    newOrder.fill(resp, buyMax->data);
                    buyMax->orderQueue.front().fill(resp, buyMax->data);
                    buyMax->orderQueue.pop();
                    if(buyMax->orderQueue.empty()){
                        BuyingDataTree[instrument].deleteNode(buyMax->data);
                    }
                    quantity = 0;
                    break;

                } else if (quantity < availQuantity){
                    newOrder.fill(resp, buyMax->data);
                    buyMax->orderQueue.front().pfill(resp, quantity, buyMax->data);
                    quantity = 0;
                    break;

                } else{
                    quantity -= availQuantity;
                    newOrder.pfill(resp, availQuantity, buyMax->data);
                    buyMax->orderQueue.front().fill(resp, buyMax->data);
                    buyMax->orderQueue.pop();
                    if(buyMax->orderQueue.empty()){
                        BuyingDataTree[instrument].deleteNode(buyMax->data);
                    }
                    buyMax = BuyingDataTree[instrument].maxValue();
                    if (buyMax->data < price){
                        break;
                    }
                    availQuantity = buyMax->orderQueue.front().getQuan();
                }
            }
            if(quantity){
                newOrder.setQuantity(quantity);
                std::queue<Order> *pQueue = SellingDataTree[instrument].insert(price);
                pQueue->push(newOrder);
            }

        }
//        std::cout << resp.str();

    }


    // That dummies are avoiding some errors with the red black tree.
    // That avoid the no element in the tree state
    // In that point pushing -1.
    void pushDummiestToBuy() {
        for (auto & i : BuyingDataTree) {
            i.insert(-1.0);
        }
    }


    // That codes are used for handle Buy requests without any errors

    void handleBuy(Order& newOrder, int instrument, double price, int quantity, std::vector<std::string> &resp) {
       // std::resptringstream resp;
        //Get the minimum price selling Order
        Node* SellMin = SellingDataTree[instrument].minValue();
        //If it higher than the Buyer order Price, It add to the tree without executing
        if(SellMin->data > price) {
            std::queue<Order> *pQueue = BuyingDataTree[instrument].insert(price);
            pQueue->push(newOrder);
            newOrder.newOrd(resp);
        } else{
            //for handle
            int StartQuantity = quantity;
            int availQuantity = SellMin->orderQueue.front().getQuan();

            while (true){

                // getting the maximum buying request

                if(availQuantity == quantity){
                    newOrder.fill(resp, SellMin->data);
                    SellMin->orderQueue.front().fill(resp, SellMin->data);
                    SellMin->orderQueue.pop();
                    if(SellMin->orderQueue.empty()){
                        SellingDataTree[instrument].deleteNode(SellMin->data);
                    }
                    quantity = 0;
                    break;

                } else if (quantity < availQuantity){
                    newOrder.fill(resp, SellMin->data);
                    SellMin->orderQueue.front().pfill(resp, quantity, SellMin->data);
                    quantity = 0;
                    break;

                } else{
                    quantity -= availQuantity;
                    newOrder.pfill(resp, availQuantity, SellMin->data);
                    SellMin->orderQueue.front().fill(resp, SellMin->data);
                    SellMin->orderQueue.pop();
                    if(SellMin->orderQueue.empty()){
                        SellingDataTree[instrument].deleteNode(SellMin->data);
                    }
                    SellMin = SellingDataTree[instrument].minValue();
                    if (SellMin->data > price){
                        break;
                    }
                    availQuantity = SellMin->orderQueue.front().getQuan();
                }
            }
            if(quantity){
                newOrder.setQuantity(quantity);
                std::queue<Order> *pQueue = BuyingDataTree[instrument].insert(price);
                pQueue->push(newOrder);
            }

        }
//        std::cout << resp.str();
    }
}