#ifndef BACKEND_VALIDATEORDER_H
#define BACKEND_VALIDATEORDER_H

#include <iostream>
#include "RedBlackTree.h"
#include "order.h"
#include "AllTrees.h"

namespace sdds{
    void processOrder(const std::string &clientOrderID, std::string instrument, std::string side, std::string quantity,
                      std::string price, std::ostringstream &os);
    bool isDouble(const std::string& str);
    bool isInteger(const std::string& str);
}

#endif //BACKEND_VALIDATEORDER_H
