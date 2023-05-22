#include "login.h"
#include <iomanip>
#include "utils.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

void handleLogin(served::response& res, const served::request& req) {

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
            if (form_data.find("username") == form_data.end()) {
                missing_fields.push_back("username");
            }
            if (form_data.find("password") == form_data.end()) {
                missing_fields.push_back("password");
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

                std::string username = form_data["username"];
                std::string password = form_data["password"];
                std::string sessionToken = generateToken();

                // Check for empty fields
                std::vector<std::string> missingFields;
                if (username.empty()) {
                    missingFields.push_back("username");
                }
                if (password.empty()) {
                    missingFields.push_back("password");
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
                    // Query the database to get the hashed password for the given username
                    std::string getPasswordQuery = "SELECT password FROM users WHERE username = ?";
                    sql::PreparedStatement *stmt = con->prepareStatement(getPasswordQuery);
                    stmt->setString(1, username);
                    sql::ResultSet *resultSet = stmt->executeQuery();

                    if (resultSet->next()) {
                        std::string hashedPassword = resultSet->getString("password");

                        // Compare the provided password with the hashed password
                        bool passwordMatch = validateSHA256Hash(password, hashedPassword);

                        if (passwordMatch) {
                            // Get the user ID from the database
                            std::string getId = "SELECT id FROM users WHERE username = ?";
                            sql::PreparedStatement *stmt2 = con->prepareStatement(getId);
                            stmt2->setString(1, username);
                            sql::ResultSet *resultSet2 = stmt2->executeQuery();

                            if (resultSet2->next()) {
                                int userId = resultSet2->getInt("id");

                                // Get the current login time
                                time_t now = time(0);
                                struct tm* loginTime = localtime(&now);
                                std::stringstream loginTimeStream;
                                loginTimeStream << std::put_time(loginTime, "%Y-%m-%d %H:%M:%S");
                                std::string loginTimeString = loginTimeStream.str();

                                // Prepare the UPDATE statement
                                std::string updateQuery = "UPDATE users SET token = ?, login_time = ? WHERE id = ?";

                                // Execute the UPDATE statement
                                stmt = con->prepareStatement(updateQuery);

                                stmt->setString(1, sessionToken);
                                stmt->setString(2, loginTimeString);
                                stmt->setInt(3, userId);

                                stmt->executeUpdate();

                                delete stmt2;

                                // Create a JSON response with user ID and token
                                rapidjson::Document jsonResponse;
                                jsonResponse.SetObject();
                                jsonResponse.AddMember("userId", userId, jsonResponse.GetAllocator());
                                jsonResponse.AddMember("token", rapidjson::Value(sessionToken.c_str(), sessionToken.size()), jsonResponse.GetAllocator());

                                // Serialize the JSON data to a string
                                rapidjson::StringBuffer buffer;
                                rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
                                jsonResponse.Accept(writer);

                                // Return a success response with the JSON data
                                res.set_status(200);
                                res.set_header("Content-Type", "application/json");
                                res << buffer.GetString();
                            } else {
                                // User not found
                                res.set_status(404); // Not Found status code
                                res.set_header("Content-Type", "application/json");
                                res << R"({"error": "Invalid user"})";
                            }
                        } else {
                            // Password is incorrect
                            res.set_status(401); // Unauthorized status code
                            res.set_header("Content-Type", "application/json");
                            res << R"({"error": "Incorrect password"})";
                        }
                    } else {
                        // User not found
                        res.set_status(404); // Not Found status code
                        res.set_header("Content-Type", "application/json");
                        res << R"({"error": "Invalid user"})";
                    }

                    delete resultSet;
                    delete stmt;
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
