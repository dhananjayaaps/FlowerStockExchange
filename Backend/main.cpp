#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "processOrder.h"
#include "AllTrees.h"
#include "BuyHandler.h"

using namespace sdds;

int main() {

    std::cout << "Welcome to the flower Stock exchange"<< std::endl;
    std::ifstream file("order.csv");

    pushDummiestToBuy();
    pushDummiestToSell();

    std::string line;

    //ignore the fist line
    std::getline(file, line);

    std::string clientOrderID, instrument , side, quantity, price;

    // Read each line of the CSV file
    while (std::getline(file, line)) {
        std::stringstream ss(line);

        // Parse each value in the line
        std::getline(ss, clientOrderID, ',');
        std::getline(ss, instrument, ',');
        std::getline(ss, side, ',');
        std::getline(ss, quantity, ',');
        std::getline(ss, price, ',');


        // Call the function with the Order as argument
        processOrder(clientOrderID, instrument, side, quantity, price);
    }

    return 0;
}
