#include "order.h"

#include <utility>
#include <chrono>
#include <iomanip>
#include <sstream>

namespace sdds{

    Order::Order(){
        m_orderId = 0;
        m_clientOrderID = nullptr;
        m_instrument = 0;
        m_side = 0;
        m_quantity = 0;
        m_price = 0;
    }

    Order::Order(std::string id, int instrument, int side, int quantity, double price) {
        m_orderId = ++orderID;
        m_clientOrderID = std::move(id);
        m_instrument = instrument;
        m_side = side;
        m_quantity = quantity;
        m_price = price;
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
        auto now = std::chrono::system_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);

        // Convert to local time
        std::tm local_time = *std::localtime(&now_c);

        // Format time as string and append to output stream
        os << std::put_time(&local_time, "%Y-%m-%d %H:%M:%S");
        os << '.' << std::setfill('0') << std::setw(3) << ms.count();
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
        auto now = std::chrono::system_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);

        // Convert to local time
        std::tm local_time = *std::localtime(&now_c);

        // Format time as string and append to output stream
        os << std::put_time(&local_time, "%Y-%m-%d %H:%M:%S");
        os << '.' << std::setfill('0') << std::setw(3) << ms.count();
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
        auto now = std::chrono::system_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);

        // Convert to local time
        std::tm local_time = *std::localtime(&now_c);

        // Format time as string and append to output stream
        os << std::put_time(&local_time, "%Y-%m-%d %H:%M:%S");
        os << '.' << std::setfill('0') << std::setw(3) << ms.count();
        os<<",";
        resp.push_back(os.str());
    }

    void Order:: setQuantity(int quantity){
        m_quantity = quantity;
    }

}

