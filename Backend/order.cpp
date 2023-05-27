#include "order.h"
#include <utility>
#include <sstream>
#include "Cache.h"

namespace sdds{

    Order::Order(){
        m_orderId = 0;
        m_clientOrderID = nullptr;
        m_instrument = 0;
        m_side = 0;
        m_quantity = 0;
        m_price = 0;
    }

    Order::Order(std::string id, int instrument, int side, int quantity, double price, int uid) {
        incrementCount();
        m_orderId = getCount();
        m_clientOrderID = std::move(id);
        m_instrument = instrument;
        m_side = side;
        m_quantity = quantity;
        m_price = price;
        userId = uid;
    }

    int Order::getQuan(){
        return m_quantity;
    }

    void Order::fill(std::vector<std::string> &resp, double price) const {
        std::stringstream os;
        os << m_orderId << ",";
        os << m_clientOrderID << ",";
        os << instrumentName[m_instrument] << ",";
        os << m_side << ",";
        os << "Fill" << ",";
        os << m_quantity << ",";
        os << price << ",";

        // Get current system time with milliseconds
        os << getTime();
        os<<",";
        resp.push_back(os.str());
    }


    void Order::newOrd(std::vector<std::string> &resp) const {
        std::stringstream os;
        os << m_orderId << ",";
        os<<m_clientOrderID<<",";
        os<<instrumentName[m_instrument]<<",";
        os<<m_side<<",";
        os<<"New"<<",";
        os<<m_quantity<<",";
        os<<m_price <<",";

        // Get current system time with milliseconds
        os << getTime();
        os<<",";
        resp.push_back(os.str());
    }

    void Order::pfill(std::vector<std::string> &resp, int quantityRm, double price) {
        std::stringstream os;
        m_quantity -= quantityRm;
        os << m_orderId << ",";
        os<<m_clientOrderID<<",";
        os<<instrumentName[m_instrument]<<",";
        os<<m_side<<",";
        os<<"pfill"<<",";
        os<<quantityRm<<",";
        os<<price << ",";

        // Get current system time with milliseconds
        os << getTime();
        os<<",";
        resp.push_back(os.str());
    }

    void Order:: setQuantity(int quantity){
        m_quantity = quantity;
    }

}

