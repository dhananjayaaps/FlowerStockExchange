#include "handleOrder.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include <iostream>
#include <served/request.hpp>
#include <served/response.hpp>
#include <vector>
#include "ValidateOrder.h"

bool validateToken(const std::string& token, const std::string& userId, served::response &res){
    // Database connection setup
    sql::mysql::MySQL_Driver *driver;
    sql::Connection *con;
    driver = sql::mysql::get_mysql_driver_instance();
    con = driver->connect(DB_HOST, DB_USER, DB_PASSWORD);
    con->setSchema(DB_NAME);

    std::string gettokenQuery = "SELECT token FROM users WHERE id = ?";
    sql::PreparedStatement *stmt = con->prepareStatement(gettokenQuery);
    stmt->setString(1, userId);
    sql::ResultSet *resultSet = stmt->executeQuery();

    if (resultSet->next()) {
//                        std::string hashedtoken = resultSet->getString("token");
//
//                        // Compare the provided token with the hashed token
//                        bool tokenMatch = validateSHA256Hash(token, hashedtoken);
        std::string normalToken = resultSet->getString("token");

        if (normalToken == token) {
            // Tokens match
            return true;
        }
        else {
            // token is incorrect
            res.set_status(401);
            return false;
        }
    } else {
        // user not found
        res.set_status(404);
        return false;
    }
};

void newOrder(served::response &res, const served::request &req){
    try {
        std::string Token = req.header("Token");  // Replace "Token" with the actual header key for your token
        std::string userID = req.header("ID");  // Replace "User-ID" with the actual header key for your user ID

// Validate token and user ID
        if (!validateToken(Token, userID, res)) {
            res.set_header("content-type", "application/json");
            res << R"({ "error": "Unauthorized" })";
            return;
        }

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
            sdds::processOrder(Client_ord_ID, Instrument, side, quantity, price, data);

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
        res << R"({ "error": ")" << e.what() << "\" }";
    }
};

