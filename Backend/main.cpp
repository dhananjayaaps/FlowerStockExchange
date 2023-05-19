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
    std::cout << "Welcome to the flower Stock exchange"<< std::endl;
    pushDummiestToBuy();
    pushDummiestToSell();
    orderID = 0;

    std::string public_ip = "10.0.0.4";
    if (public_ip.empty()) {
        std::cerr << "Failed to retrieve public IP address." << std::endl;
        return EXIT_FAILURE;
    }

    served::multiplexer mux;

    mux.handle("/api/order/new")
    .post([&](served::response &res, const served::request &req) {
        try {
            if (req.header("Content-Type").find("application/x-www-form-urlencoded") == 0) {
                std::string body = req.body();
                std::unordered_map<std::string, std::string> form_data;

                // Parse form data
                std::string delimiter = "&";
                size_t pos = 0;
                std::string token;
                while ((pos = body.find(delimiter)) != std::string::npos) {
                    token = body.substr(0, pos);
                    size_t equal_pos = token.find('=');
                    if (equal_pos != std::string::npos) {
                        std::string key = token.substr(0, equal_pos);
                        std::string value = token.substr(equal_pos + 1);
                        form_data[key] = value;
                    }
                    body.erase(0, pos + delimiter.length());
                }
                // Parse the last key-value pair
                size_t equal_pos = body.find('=');
                if (equal_pos != std::string::npos) {
                    std::string key = body.substr(0, equal_pos);
                    std::string value = body.substr(equal_pos + 1);
                    form_data[key] = value;
                }

                // Validate required fields
                std::vector<std::string> missing_fields;
                if (form_data.find("Client_ID") == form_data.end()) {
                    missing_fields.push_back("Client_ID");
                }
                if (form_data.find("Instrument") == form_data.end()) {
                    missing_fields.push_back("Instrument");
                }
                if (form_data.find("Side") == form_data.end()) {
                    missing_fields.push_back("Side");
                }
                if (form_data.find("Quantity") == form_data.end()) {
                    missing_fields.push_back("Quantity");
                }
                if (form_data.find("Price") == form_data.end()) {
                    missing_fields.push_back("Price");
                }

                if (!missing_fields.empty()) {
                    std::string error_message = "Invalid request format or missing required fields: ";
                    for (const auto& field : missing_fields) {
                        error_message += field + ", ";
                    }
                    error_message = error_message.substr(0, error_message.length() - 2);  // Remove the trailing comma and space
                    throw std::runtime_error(error_message);
                }

                // Extract necessary data
                std::string Client_ord_ID = form_data["Client_ID"];
                std::string Instrument = form_data["Instrument"];
                std::string side = form_data["Side"];
                std::string quantity = form_data["Quantity"];
                std::string price = form_data["Price"];

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

            }
            else{
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
            }

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

    return 0;
}
