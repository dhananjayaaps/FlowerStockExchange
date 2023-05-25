#include <iostream>
#include <fstream>
#include <served/served.hpp>
#include <string>
#include "HandlerLogic.h"
#include "count.h"
#include "handleOrder.h"

using namespace sdds;

int main() {
    std::cout << "Welcome to the flower Stock exchange"<< std::endl;
    pushDummiestToBuy();
    pushDummiestToSell();
    orderID = 0;

    std::string public_ip = "127.0.0.1";
    if (public_ip.empty()) {
        std::cerr << "Failed to retrieve public IP address." << std::endl;
        return EXIT_FAILURE;
    }

    served::multiplexer mux;

    mux.handle("/api/order/new")
    .post([&](served::response &res, const served::request &req) {
        newOrder(res, req);
    });


    std::cout << "Try this example with:" << std::endl;
    std::cout << "  curl \"http://" << public_ip << ":/api/order/new\"" << std::endl;

    served::net::server server(public_ip, "8080", mux);
    server.run(10);

    return 0;
}
