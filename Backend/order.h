#ifndef BACKEND_ORDER_H
#define BACKEND_ORDER_H

#include <iostream>
#include <cstring>
#include <vector>

namespace sdds{

    class Order {

        char instrumentName[5][9] = {"Rose", "Lavender" , "Lotus" , "Tulip" ,"Orchid" };

        Order();
        int m_orderId{};
        int m_instrument{};
        int m_side{};
        int m_quantity{};
        double m_price{};
        int userId{};

    public:
        Order(std::string id, int instrument, int side, int quantity, double price, int userId);

        std::string m_clientOrderID{};

        int getQuan();
        void fill(std::vector<std::string> &resp, double) const;
        void newOrd(std::vector<std::string> &resp) const;
        void pfill(std::vector<std::string> &resp, int quantityRm, double);
        void setQuantity(int quantity);
    };
}


#endif //BACKEND_ORDER_H
