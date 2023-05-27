#include <iostream>
#include <fstream>
#include <served/served.hpp>
#include <string>
#include "HandlerLogic.h"
#include "count.h"
#include "handleOrder.h"
#include "getHistory.h"

using namespace sdds;

int main() {
    std::cout << "Welcome to the flower Stock exchange"<< std::endl;
    pushDummiestToBuy();
    pushDummiestToSell();
    orderID = 0;

    //std::string public_ip = "127.0.0.1";
    std::string public_ip = "10.0.0.4";
    if (public_ip.empty()) {
        std::cerr << "Failed to retrieve public IP address." << std::endl;
        return EXIT_FAILURE;
    }

    served::multiplexer mux;

    mux.handle("/api/order/new")
        .post([&](served::response &res, const served::request &req) {
            newOrder(res, req);
        });

    mux.handle("/api/order/get")
        .get([&](served::response &res, const served::request &req) {
                getOrderHistory(res, req);
            });


    std::cout << "server is working on port 8080" << std::endl;

    served::net::server server(public_ip, "8080", mux);
    server.run(10);

    return 0;
}
