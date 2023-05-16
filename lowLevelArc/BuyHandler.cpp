#include "BuyHandler.h"
#include <ostream>
#include <sstream>
#include <limits>

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

    void handleSell(Order &newOrder, int instrument, double price, int quantity, std::ostringstream &os) {
//        std::ostringstream os;

        //Get the maximum Buying Order
        Node* buyMax = BuyingDataTree[instrument].maxValue();

        //If it lower than the Selling order Price, It add to the tree without executing
        if(buyMax->data < price) {

            std::queue<Order> *pQueue = SellingDataTree[instrument].insert(price);
            pQueue->push(newOrder);
            newOrder.newOrd(os);
            os<<std::endl;
        } else{

            //for handle
            int StartQuantity = quantity;
            int availQuantity = buyMax->orderQueue.front().getQuan();

            while (true){

                // getting the maximum buying request

                if(availQuantity == quantity){
                    newOrder.fill(os, buyMax->data);
                    os<<std::endl;
                    buyMax->orderQueue.front().fill(os, buyMax->data);
                    os<<std::endl;
                    buyMax->orderQueue.pop();
                    if(buyMax->orderQueue.empty()){
                        BuyingDataTree[instrument].deleteNode(buyMax->data);
                    }
                    quantity = 0;
                    break;

                } else if (quantity < availQuantity){
                    newOrder.fill(os, buyMax->data);
                    os << std::endl;
                    buyMax->orderQueue.front().pfill(os, quantity, buyMax->data);
                    os << std::endl;
                    quantity = 0;
                    break;

                } else{
                    quantity -= availQuantity;
                    newOrder.pfill(os, availQuantity, buyMax->data);
                    os << std::endl;
                    buyMax->orderQueue.front().fill(os, buyMax->data);
                    buyMax->orderQueue.pop();
                    os << std::endl;
                    if(buyMax->orderQueue.empty()){
                        BuyingDataTree[instrument].deleteNode(buyMax->data);
                    }
                    buyMax = BuyingDataTree[instrument].maxValue();
                    if (buyMax->data < price){
                        break;
                    }
                }
            }
            if(quantity){
                newOrder.setQuantity(quantity);
                std::queue<Order> *pQueue = SellingDataTree[instrument].insert(price);
                pQueue->push(newOrder);
            }

        }
//        std::cout << os.str();

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

    void handleBuy(Order& newOrder, int instrument, double price, int quantity, std::ostringstream &os) {
       // std::ostringstream os;
        //Get the minimum price selling Order
        Node* SellMin = SellingDataTree[instrument].minValue();
        //If it higher than the Buyer order Price, It add to the tree without executing
        if(SellMin->data > price) {
            std::queue<Order> *pQueue = BuyingDataTree[instrument].insert(price);
            pQueue->push(newOrder);
            newOrder.newOrd(os);
            os << std::endl;
        } else{
            //for handle
            int StartQuantity = quantity;
            int availQuantity = SellMin->orderQueue.front().getQuan();

            while (true){

                // getting the maximum buying request

                if(availQuantity == quantity){
                    newOrder.fill(os, SellMin->data);
                    os<<std::endl;
                    SellMin->orderQueue.front().fill(os, SellMin->data);
                    os<<std::endl;
                    SellMin->orderQueue.pop();
                    if(SellMin->orderQueue.empty()){
                        SellingDataTree[instrument].deleteNode(SellMin->data);
                    }
                    quantity = 0;
                    break;

                } else if (quantity < availQuantity){
                    newOrder.fill(os, SellMin->data);
                    os << std::endl;
                    SellMin->orderQueue.front().pfill(os, quantity, SellMin->data);
                    os << std::endl;
                    quantity = 0;
                    break;

                } else{
                    quantity -= availQuantity;
                    newOrder.pfill(os, availQuantity, SellMin->data);
                    os << std::endl;
                    SellMin->orderQueue.front().fill(os, SellMin->data);
                    SellMin->orderQueue.pop();
                    os << std::endl;
                    if(SellMin->orderQueue.empty()){
                        SellingDataTree[instrument].deleteNode(SellMin->data);
                    }
                    SellMin = SellingDataTree[instrument].minValue();
                    if (SellMin->data > price){
                        break;
                    }
                }
            }
            if(quantity){
                newOrder.setQuantity(quantity);
                std::queue<Order> *pQueue = BuyingDataTree[instrument].insert(price);
                pQueue->push(newOrder);
            }

        }
//        std::cout << os.str();
    }
}