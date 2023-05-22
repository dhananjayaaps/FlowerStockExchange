#ifndef AUTHENTICATER_SIGNUP_H
#define AUTHENTICATER_SIGNUP_H

#pragma once

#include <served/served.hpp>
#include "databaseDetails.h"
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <mysql_driver.h>
#include <mysql_connection.h>

void handleSignup(served::response& res, const served::request& req);

#endif //AUTHENTICATER_SIGNUP_H
