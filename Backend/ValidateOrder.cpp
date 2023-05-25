#include "ValidateOrder.h"
#include <iostream>
#include <algorithm>
#include <regex>
#include <chrono>
#include <iomanip>
#include "HandlerLogic.h"

namespace sdds{

    void raiseError(const std::string &Client_ID, const std::string &Instrument, const std::string &side,
                          const std::string &quantity, const std::string &price, const std::exception& e, std::vector<std::string> &resp) {
        std::stringstream os;
        os << ++orderID << ",";
        os<<Client_ID<<",";
        os<<Instrument<<",";
        os<<side<<",";
        os<<"Rejected"<<",";
        os<<quantity<<",";
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
        os << ",";
        os << e.what();
        resp.push_back(os.str());
    }


    bool isDouble(const std::string& str) {
        int dotCount = 0;
        for (char c : str) {
            if (!isdigit(c)) {
                if(c!='.' || dotCount >0){
                    return false;
                }
                else{
                    dotCount ++;
                }
            }
        }
        return true;
    }

    bool isInteger(const std::string& str) {
        for (char c : str) {
            if (!isdigit(c)) {
                return false;
            }
        }
        return true;
    }

    // process each input from user
    void processOrder(const std::string& clientOrderID,const std::string& instrument,const std::string& side,const std::string& quantity,const std::string& price,std::vector<std::string>& data){


    int m_side, m_quantity;
        double m_price;

        // Convert side, quantity, and price to integers
        try {
            int t_instrument;

            //for ease, I used
            //Rose = 0
            //Lavender = 1
            //Lotus = 2
            //Tulip = 3
            //Orchid = 4
            // Then call the constructor for the make an object

            switch (instrument[0]) {
                case 'R':
                    if (instrument == "Rose") {
                        t_instrument = 0;
                        break;
                    }
                case 'L':
                    if (instrument == "Lavender") {
                        t_instrument = 1;
                        break;
                    } else if (instrument == "Lotus") {
                        t_instrument = 2;
                        break;
                    }
                case 'T':
                    t_instrument = 3;
                    if (instrument == "Tulip") {
                        break;
                    }
                case 'O':
                    t_instrument = 4;
                    if (instrument == "Orchid") {
                        break;
                    }

                default:
                    throw std::invalid_argument("Invalid instrument.");
            }

            if (clientOrderID.empty()){
                throw std::invalid_argument("Invalid clientOrderID");
            }

            // validate the side argument

            if (!isInteger(side) || side.empty()) {
                throw std::invalid_argument("Invalid Side");
            }

            m_side = std::stoi(side);
            if(m_side != 1 && m_side != 2 ){
                throw std::invalid_argument("Invalid Side");
            }
            // validate quantity

            if (!isInteger(quantity) || quantity.empty()) {
                throw std::invalid_argument("Invalid quantity");
            }

            m_quantity = std::stoi(quantity);
            if (not(m_quantity >= 10 && m_quantity <= 1000 && m_quantity%10 == 0)){
                throw std::invalid_argument("Invalid quantity");
            }

            // validate price

            if (!isDouble(price) || price.empty()){
                throw std::invalid_argument("Invalid price");
            }


            m_price = std::stod(price);

            if(m_price <= 0){
                throw std::invalid_argument("Invalid price.");
            }

            if(m_side == 1){
                Order newBuy(clientOrderID, t_instrument, m_side, m_quantity, m_price);
                handleBuy(newBuy, t_instrument, m_price, m_quantity, data);
            } else{
                Order newSell(clientOrderID, t_instrument, m_side, m_quantity, m_price);
                handleSell(newSell, t_instrument, m_price, m_quantity, data);
            }
        }
        catch (const std::exception& e) {

            raiseError(clientOrderID, instrument,side,quantity,price,e,data);
        }

    }
}
