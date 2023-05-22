#include <iomanip>
#include "signup.h"
#include "utils.h"

void handleSignup(served::response& res, const served::request& req) {
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
            if (form_data.find("username") == form_data.end()) {
                missing_fields.push_back("username");
            }
            if (form_data.find("firstName") == form_data.end()) {
                missing_fields.push_back("firstName");
            }
            if (form_data.find("lastName") == form_data.end()) {
                missing_fields.push_back("lastName");
            }
            if (form_data.find("phoneNumber") == form_data.end()) {
                missing_fields.push_back("phoneNumber");
            }
            if (form_data.find("email") == form_data.end()) {
                missing_fields.push_back("email");
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
                std::string firstName = form_data["firstName"];
                std::string lastName = form_data["lastName"];
                std::string phoneNumber = form_data["phoneNumber"];
                std::string email = form_data["email"];
                std::string password = form_data["password"];
                std::string role = "client";
                std::string sessionToken = generateToken();

                email = decodeEmail(email);
                password = decodeEmail(password);

                // Check for empty fields
                std::vector<std::string> missingFields;
                if (username.empty()) {
                    missingFields.push_back("username");
                }
                if (firstName.empty()) {
                    missingFields.push_back("firstName");
                }
                if (lastName.empty()) {
                    missingFields.push_back("lastName");
                }
                if (phoneNumber.empty()) {
                    missingFields.push_back("phoneNumber");
                }
                if (email.empty()) {
                    missingFields.push_back("email");
                }
                if (password.empty()) {
                    missingFields.push_back("password");
                }
                if (role.empty()) {
                    missingFields.push_back("role");
                }

                std::vector<std::string> validity;

                if (!isEmailValid(email)) {
                    validity.push_back("email");
                    std::cout << "invalid";
                }
                else
                {
                    std::cout << "valid";
                }

                std::string hashedPassword = sha256Hash(password);

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

                } else if (!validity.empty()) {
                    // Return an error response with missing fields message
                    res.set_status(400);
                    res.set_header("Content-Type", "application/json");
                    res << R"({"error": "Invalid Content", "Invalid Contents": [)";
                    for (size_t i = 0; i < validity.size(); ++i) {
                        res << "\"" << validity[i] << "\"";
                        if (i < validity.size() - 1) {
                            res << ",";
                        }
                    }
                    res << "]}";
                    return;
                }

                else{
                    // Get the current login time
                    time_t now = time(0);
                    struct tm* loginTime = localtime(&now);
                    std::stringstream loginTimeStream;
                    loginTimeStream << std::put_time(loginTime, "%Y-%m-%d %H:%M:%S");
                    std::string loginTimeString = loginTimeStream.str();

                    // Establish a connection to the MySQL database
                    sql::mysql::MySQL_Driver *driver;
                    sql::Connection *con;
                    driver = sql::mysql::get_mysql_driver_instance();
                    con = driver->connect(DB_HOST, DB_USER, DB_PASSWORD);
                    con->setSchema(DB_NAME);

                    // Prepare the INSERT statement
                    sql::PreparedStatement *stmt = con->prepareStatement("INSERT INTO users (username, first_name, last_name, phone_number, email, password, role, token, login_time) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)");
                    stmt->setString(1, username);
                    stmt->setString(2, firstName);
                    stmt->setString(3, lastName);
                    stmt->setString(4, phoneNumber);
                    stmt->setString(5, email);
                    stmt->setString(6, hashedPassword);
                    stmt->setString(7, role);
                    stmt->setString(8, sessionToken);
                    stmt->setString(9, loginTimeString);

                    // Execute the INSERT statement
                    stmt->executeUpdate();

                    // Clean up
                    delete stmt;
                    delete con;

                    // Return a success response
                    res.set_status(200);
                    res.set_header("Content-Type", "application/json");
                    res << R"({"message": "User inserted successfully"})";
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
