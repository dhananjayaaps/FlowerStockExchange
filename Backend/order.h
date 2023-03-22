#ifndef BACKEND_ORDER_H
#define BACKEND_ORDER_H

#include <iostream>
#include <cstring>

namespace sdds{
    class Order {


        Order();

        int m_instrument{};
        int m_side{};
        int m_quantity{};
        double m_price{};

    public:
        Order(std::string id, int instrument, int side, int quantity, double price);

        std::string m_clientOrderID{};

        int getQuan();
        std::ostream& fill(std::ostream& os) const;
        std::ostream& newOrd(std::ostream& os) const;
        std::ostream& pfill(std::ostream& os, int quantityRm);
    };
}


#endif //BACKEND_ORDER_H
