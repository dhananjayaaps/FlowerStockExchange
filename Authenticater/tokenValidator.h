#ifndef AUTHENTICATER_TOKENVALIDATOR_H
#define AUTHENTICATER_TOKENVALIDATOR_H


#include <served/served.hpp>
#include "databaseDetails.h"
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <mysql_driver.h>
#include <mysql_connection.h>

void tokenValidator(served::response& res, const served::request& req);


#endif //AUTHENTICATER_TOKENVALIDATOR_H
