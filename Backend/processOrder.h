#ifndef BACKEND_PROCESSORDER_H
#define BACKEND_PROCESSORDER_H

#include <iostream>
#include "RedBlackTree.h"
#include "order.h"
#include "AllTrees.h"

namespace sdds{
    void processOrder(const std::string& clientOrderID, std::string instrument, std::string side, std::string quantity, std::string price);
}

#endif //BACKEND_PROCESSORDER_H
