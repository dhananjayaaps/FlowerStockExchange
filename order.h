#ifndef BACKEND_ORDER_H
#define BACKEND_ORDER_H

#include <iostream>
#include <cstring>

namespace sdds{

    class Order {

        char instrumentName[5][9] = {"Rose", "Lavender" , "Lotus" , "Tulip" ,"Orchid" };

        Order();
        int m_orderId;
        std::string m_clientOrderID{};
        int m_instrument{};
        int m_side{};
        int m_quantity{};
        double m_price{};

    public:
        Order(std::string id, int instrument, int side, int quantity, double price);
        int getQuan();
        std::ostream& fill(std::ostream& os, double) const;
        std::ostream& newOrd(std::ostream& os) const;
        std::ostream& pfill(std::ostream& os, int quantityRm, double);
        void setQuantity(int quantity);
    };
}


#endif //BACKEND_ORDER_H
