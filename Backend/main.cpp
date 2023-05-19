#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>
#include <served/served.hpp>
#include <string>
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "ValidateOrder.h"
#include "AllTrees.h"
#include "BuyHandler.h"
#include "count.h"

using namespace sdds;

int main() {
    auto start_time = std::chrono::high_resolution_clock::now();

    std::cout << "Welcome to the flower Stock exchange"<< std::endl;
    pushDummiestToBuy();
    pushDummiestToSell();
    orderID = 0;

    auto end_time = std::chrono::high_resolution_clock::now();
    auto time_diff = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    std::string public_ip = "10.0.0.4";
    if (public_ip.empty()) {
        std::cerr << "Failed to retrieve public IP address." << std::endl;
        return EXIT_FAILURE;
    }

    served::multiplexer mux;

    mux.handle("/api/order/new")
    .post([&](served::response &res, const served::request &req) {
        try {
            std::string body = req.body();
            rapidjson::Document json_data;
            json_data.Parse(body.c_str());

            // Check if JSON parsing was successful
            if (json_data.HasParseError()) {
                throw std::runtime_error("Invalid JSON format in the request.");
            }

            // Validate required fields
            if (!json_data.IsObject() || !json_data.HasMember("Client_ID") ||
                !json_data.HasMember("Instrument") || !json_data.HasMember("Side") ||
                !json_data.HasMember("Quantity") || !json_data.HasMember("Price")) {
                throw std::runtime_error("Invalid request format or missing required fields.");
            }

            // Parse request body to extract necessary data
            json_data.Parse(body.c_str());
            std::string Client_ord_ID = json_data["Client_ID"].GetString();
            std::string Instrument = json_data["Instrument"].GetString();
            std::string side = json_data["Side"].GetString();
            std::string quantity = json_data["Quantity"].GetString();
            std::string price = json_data["Price"].GetString();

            // Add the new order to the data vector
            std::vector<std::string> data;
            processOrder(Client_ord_ID, Instrument, side, quantity, price, data);

            rapidjson::Document response_data(rapidjson::kArrayType);
            for (const auto &row : data) {
                response_data.PushBack(rapidjson::StringRef(row.c_str()), response_data.GetAllocator());
            }

            // Send the updated data as the response body
            rapidjson::StringBuffer buffer;
            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
            response_data.Accept(writer);
            res.set_header("content-type", "application/json");
            res << buffer.GetString();
        } catch (const std::exception &e) {
            // Send a bad response if an error occurs
            res.set_status(400);
            res.set_header("content-type", "application/json");
            res << "{ \"error\": \"" << e.what() << "\" }";
        }
    });


    std::cout << "Try this example with:" << std::endl;
    std::cout << "  curl \"http://" << public_ip << ":/api/order/new\"" << std::endl;

    served::net::server server(public_ip, "8080", mux);
    server.run(10);


    // Print the time difference
    std::cout << "Time difference: " << time_diff.count() << " milliseconds" << std::endl;

    return 0;
}
