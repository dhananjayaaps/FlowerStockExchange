cmake_minimum_required(VERSION 3.5)
project(cpprestserver LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
project(Backend)

add_executable(Backend main.cpp ValidateOrder.cpp order.cpp RedBlackTree.h HandlerLogic.cpp RedBlackTree.cpp order.csv Cache.cpp Cache.h Cache.h handleOrder.cpp handleOrder.h getHistory.cpp getHistory.h)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SERVED_PKG REQUIRED IMPORTED_TARGET served)

set(MYSQLCPPCONN_INCLUDE_DIR "/path/to/mysqlcppconn/include")
set(MYSQLCPPCONN_LIB_DIR "/path/to/mysqlcppconn/lib")

find_package(OpenSSL REQUIRED)

include_directories(${MYSQLCPPCONN_INCLUDE_DIR})

target_link_libraries(Backend PRIVATE OpenSSL::Crypto mysqlcppconn PkgConfig::SERVED_PKG)

link_directories(${MYSQLCPPCONN_LIB_DIR})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(Backend PRIVATE Threads::Threads)
