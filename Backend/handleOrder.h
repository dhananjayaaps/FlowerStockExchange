#ifndef BACKEND_HANDLEORDER_H
#define BACKEND_HANDLEORDER_H

#include <served/served.hpp>
#include "databaseDetails.h"
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <mysql_driver.h>
#include <mysql_connection.h>

void newOrder(served::response &res, const served::request &req);

#endif //BACKEND_HANDLEORDER_H
