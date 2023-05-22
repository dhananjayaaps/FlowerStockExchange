#ifndef AUTHENTICATER_LOGIN_H
#define AUTHENTICATER_LOGIN_H

#include <served/served.hpp>
#include "databaseDetails.h"
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <mysql_driver.h>
#include <mysql_connection.h>

void handleLogin(served::response& res, const served::request& req);

#endif //AUTHENTICATER_LOGIN_H
