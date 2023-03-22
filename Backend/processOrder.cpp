#include "processOrder.h"
#include <iostream>
#include <algorithm>
#include <regex>
#include "BuyHandler.h"
#include "SellHandler.h"

namespace sdds{

    // process each input from user
    void processOrder(const std::string& clientOrderID, std::string instrument, std::string side, std::string quantity, std::string price){

        int m_side, m_quantity;
        double m_price;

        // Convert side, quantity, and price to integers
        try {

            if (clientOrderID.empty()){
                throw std::invalid_argument("Invalid clientOrderID");
            }

            // validate the side argument

            if (std::any_of(side.begin(), side.end(), [](char c){ return !std::isdigit(c); })) {
                throw std::invalid_argument("Invalid Side");
            }

            m_side = std::stoi(side);
            if(m_side != 1 && m_side != 2 ){
                throw std::invalid_argument("Invalid Side");
            }
            // validate quantity

            if (std::any_of(quantity.begin(), quantity.end(), [](char c){ return !std::isdigit(c); })) {
                throw std::invalid_argument("Invalid quantity");
            }

            m_quantity = std::stoi(quantity);
            if (not(m_quantity >= 10 && m_quantity <= 1000 && m_quantity%10 == 0)){
                throw std::invalid_argument("Invalid quantity");
            }

            // validate price

            std::regex regex("^[]?[0-9]*\\.?[0-9]+$");
            if (!std::regex_match(price, regex)) {
                throw std::invalid_argument("Invalid price");
            }

            m_price = std::stod(price);

            if(m_price <= 0){
                throw std::invalid_argument("Invalid price.");
            }

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
            if(m_side == 1){
                Order newBuy(clientOrderID, t_instrument, m_side, m_quantity, m_price);
                handleBuy(newBuy, t_instrument, m_price);
            } else{
                Order newSell(clientOrderID, t_instrument, m_side, m_quantity, m_price);
                handleSell(newSell, t_instrument, m_price);
            }
        }
        catch (const std::exception& e) {
            // Handle the error
            std::cerr << e.what() << std::endl;
        }

    }

}
