#include <served/response.hpp>
#include "getHistory.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include <served/request.hpp>
#include <vector>
#include "databaseDetails.h"
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <mysql_driver.h>

bool validateTheToken(const std::string& token, const std::string& userId, served::response &res){
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
        res.set_status(401);
        return false;
    }
};

void getOrderHistory(served::response& res, const served::request& req)
{
    try {
        std::string token = req.header("Token");  // Replace "Token" with the actual header key for your token
        std::string userID = req.header("ID");    // Replace "User-ID" with the actual header key for your user ID

        // Validate token and user ID
        if (!validateTheToken(token, userID, res)) {
            res.set_header("content-type", "application/json");
            res.set_status(401);
            res << R"({ "error": "Unauthorized" })";
            return;
        }

        //connect to the database
        sql::mysql::MySQL_Driver* driver;
        sql::Connection* con;
        driver = sql::mysql::get_mysql_driver_instance();
        con = driver->connect(DB_HOST, DB_USER, DB_PASSWORD);
        con->setSchema(DB_NAME);

        // SQL query for get the history
        std::string selectSql = "SELECT * FROM orders WHERE userId = ? ORDER BY date_time DESC";
        sql::PreparedStatement* pstmt = con->prepareStatement(selectSql);
        pstmt->setString(1, userID);
        sql::ResultSet* resultSet = pstmt->executeQuery();

        // Array for save responses
        std::vector<std::string> responseData;

        // Iterate and add the data to the array;
        while (resultSet->next()) {
            std::string orderData;
            orderData += resultSet->getString("order_id");
            orderData += ",";
            orderData += resultSet->getString("c_order_id");
            orderData += ",";
            orderData += resultSet->getString("instrument");
            orderData += ",";
            orderData += resultSet->getString("side");
            orderData += ",";
            orderData += resultSet->getString("quantity");
            orderData += ",";
            orderData += resultSet->getString("price");
            orderData += ",";
            orderData += resultSet->getString("date_time");
            orderData += ",";
            orderData += resultSet->getString("reason"); // Replace "extra_column" with the actual name of the additional column

            responseData.push_back(orderData);
        }

        // Clean up result set and prepared statement
        delete resultSet;
        delete pstmt;
        delete con;

        // Convert data vector to a JSON string
        rapidjson::Document responseJson(rapidjson::kArrayType);
        for (const auto& data : responseData) {
            responseJson.PushBack(rapidjson::StringRef(data.c_str()), responseJson.GetAllocator());
        }
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        responseJson.Accept(writer);
        std::string responseStr = buffer.GetString();

        // Send the JSON response
        res.set_status(200);
        res.set_header("content-type", "application/json");
        res << responseStr;

    } catch (const std::exception& e) {
        // Send a bad response for errors
        res.set_status(400);
        res.set_header("content-type", "application/json");
        res << R"({ "error": ")" << e.what() << "\" }";
    }
}


