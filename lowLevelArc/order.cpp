#include "order.h"
#include "Cache.h"
#include <utility>
#include <chrono>
#include <iomanip>

namespace sdds{

    Order::Order(){
        m_clientOrderID = nullptr;
        m_instrument = 0;
        m_side = 0;
        m_quantity = 0;
        m_price = 0;
    }

    Order::Order(std::string id, int instrument, int side, int quantity, double price) {
        orderId ++;
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
        os<<orderId<<",";
        os<<m_clientOrderID<<",";
        os<<instrumentName[m_instrument]<<",";
        os<<m_side<<",";
        os<<"Fill"<<",";
        os<<m_quantity<<",";
        os<<price <<",";

        // Get current system time with milliseconds
        auto now = std::chrono::system_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);

        // Convert to local time
        std::tm local_time = *std::localtime(&now_c);

        // Format time as string and append to output stream
        os << std::put_time(&local_time, "%Y-%m-%d %H:%M:%S");
        os << '.' << std::setfill('0') << std::setw(3) << ms.count();

        return os;
    }

    std::ostream &Order::newOrd(std::ostream &os) const {
        os<<orderId<<",";
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
        return os;
    }

    std::ostream &Order::pfill(std::ostream &os, int quantityRm, double price) {
        os<<orderId<<",";
        m_quantity -= quantityRm;
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
        return os;;
    }

    void Order:: setQuantity(int quantity){
        m_quantity = quantity;
    }

}

