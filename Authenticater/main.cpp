#include <iostream>
#include <served/served.hpp>
#include "signup.h"
#include "login.h"
#include "tokenValidator.h"


int main() {
    served::multiplexer mux;

    mux.handle("/api/users/new")
            .post([](served::response& res, const served::request& req) {
                handleSignup(res, req);
            });

    mux.handle("/api/users/login")
            .post([](served::response& res, const served::request& req) {
                handleLogin(res, req);
            });

    mux.handle("/api/users/token")
            .post([](served::response& res, const served::request& req) {
                tokenValidator(res, req);
            });


    // Start the server
    served::net::server server("127.0.0.1", "8060", mux);
    server.run(10);

    return 0;
}
