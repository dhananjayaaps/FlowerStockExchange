cmake_minimum_required(VERSION 3.0)
project(Authenticater)

set(CMAKE_CXX_STANDARD 11)

# Specify the path to the mysqlcppconn library
set(MYSQLCPPCONN_INCLUDE_DIR "/path/to/mysqlcppconn/include")
set(MYSQLCPPCONN_LIB_DIR "/path/to/mysqlcppconn/lib")

find_package(OpenSSL REQUIRED)

include_directories(${MYSQLCPPCONN_INCLUDE_DIR})

add_executable(Authenticater main.cpp signup.cpp login.cpp utils.cpp tokenValidator.cpp)

target_link_libraries(Authenticater PRIVATE OpenSSL::Crypto mysqlcppconn served)

link_directories(${MYSQLCPPCONN_LIB_DIR})

target_link_libraries(Authenticater PRIVATE -pthread)

