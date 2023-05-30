#include "order.h"
#include <utility>
#include <chrono>
#include <iomanip>
#include "Cache.h"

namespace sdds{

    Order::Order(){
        m_clientOrderID = nullptr;
        m_instrument = 0;
        m_side = 0;
        m_quantity = 0;
        m_price = 0;
    }

    Order::Order(std::string id, int instrument, int side, int quantity, double price) {
        incrementCount();
        m_orderId = getCount();
        m_clientOrderID = std::move(id);
        m_instrument = instrument;
        m_side = side;
        m_quantity = quantity;
        m_price = price;
    }

    int Order::getQuan(){
        return m_quantity;
    }

    std::ostream &Order::fill(std::ostream &os, double price) const {
        os<<m_orderId<<",";
        os<<m_clientOrderID<<",";
        os<<instrumentName[m_instrument]<<",";
        os<<m_side<<",";
        os<<"Fill"<<",";
        os<<m_quantity<<",";
        os<<price <<",";
        os << ",";
        os << getTime();

        return os;
    }

    std::ostream &Order::newOrd(std::ostream &os) const {
        os<<m_orderId<<",";
        os<<m_clientOrderID<<",";
        os<<instrumentName[m_instrument]<<",";
        os<<m_side<<",";
        os<<"New"<<",";
        os<<m_quantity<<",";
        os<<m_price <<",";
        os << ",";
        os << getTime();
        return os;
    }

    std::ostream &Order::pfill(std::ostream &os, int quantityRm, double price) {
        os<<m_orderId<<",";
        m_quantity -= quantityRm;
        os<<m_clientOrderID<<",";
        os<<instrumentName[m_instrument]<<",";
        os<<m_side<<",";
        os<<"pfill"<<",";
        os<<quantityRm<<",";
        os<<price << ",";
        os << ",";
        os << getTime();
        return os;;
    }

    void Order:: setQuantity(int quantity){
        m_quantity = quantity;
    }

}

