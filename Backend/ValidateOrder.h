#ifndef BACKEND_VALIDATEORDER_H
#define BACKEND_VALIDATEORDER_H

#include <iostream>
#include "RedBlackTree.h"
#include "order.h"
#include "AllTrees.h"

namespace sdds{

    void processOrder(const std::string& Client_ID,const std::string& Instrument,const std::string& side,const std::string& quantity,const std::string& price,std::vector<std::string>& data);
    bool isDouble(const std::string& str);
    bool isInteger(const std::string& str);
}

#endif //BACKEND_VALIDATEORDER_H
