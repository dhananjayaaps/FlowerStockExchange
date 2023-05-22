#include <iomanip>
#include "utils.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "tokenValidator.h"

void tokenValidator(served::response& res, const served::request& req) {

    try {
        // Database connection setup
        sql::mysql::MySQL_Driver *driver;
        sql::Connection *con;
        driver = sql::mysql::get_mysql_driver_instance();
        con = driver->connect(DB_HOST, DB_USER, DB_PASSWORD);
        con->setSchema(DB_NAME);

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
            if (form_data.find("userId") == form_data.end()) {
                missing_fields.push_back("userId");
            }
            if (form_data.find("token") == form_data.end()) {
                missing_fields.push_back("token");
            }


            if (!missing_fields.empty()) {
                std::string error_message = "Invalid request format or missing required fields: ";
                for (const auto& field : missing_fields) {
                    error_message += field + ", ";
                }
                error_message = error_message.substr(0, error_message.length() - 2);  // Remove the trailing comma and space
                throw std::runtime_error(error_message);
            }else{

                // Extract form data

                std::string userId = form_data["userId"];
                std::string sessionToken = form_data["token"];

                // Check for empty fields
                std::vector<std::string> missingFields;
                if (userId.empty()) {
                    missingFields.push_back("userId");
                }
                if (sessionToken.empty()) {
                    missingFields.push_back("token");
                }


                if (!missingFields.empty()) {
                    // Return an error response with missing fields message
                    res.set_status(400);
                    res.set_header("Content-Type", "application/json");
                    res << R"({"error": "Missing required fields", "missingFields": [)";
                    for (size_t i = 0; i < missingFields.size(); ++i) {
                        res << "\"" << missingFields[i] << "\"";
                        if (i < missingFields.size() - 1) {
                            res << ",";
                        }
                    }
                    res << "]}";
                    return;
                }

                else {
                    // Query the database to get the hashed token for the given userId
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

                        if (normalToken == sessionToken) {
                            // Tokens match
                            res.set_status(200);
                            res.set_header("Content-Type", "application/json");
                            res << R"({"status": 1})";
                        }
                        else {

                            res.set_status(401);
                            res.set_header("Content-Type", "application/json");
                            res << R"({"status": 0})";
                            }
                        } else {
                            // token is incorrect
                        res.set_status(404);
                        res.set_header("Content-Type", "application/json");
                        res << R"({"status": 0})";
                        }

                }

            }
        } else {
            throw std::runtime_error("Invalid request content type");
        }
    } catch (const std::exception& e) {
        res.set_status(400);
        res.set_header("Content-Type", "application/json");
        res << R"({"error": ")" << e.what() << R"("})";
    }
}

