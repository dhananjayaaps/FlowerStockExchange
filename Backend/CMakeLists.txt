cmake_minimum_required(VERSION 3.5)
project(cpprestserver LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
project(Backend)

add_executable(Backend main.cpp ValidateOrder.cpp order.cpp RedBlackTree.h BuyHandler.cpp RedBlackTree.cpp order.csv Cache.cpp Cache.h Cache.h count.h)


find_package(PkgConfig REQUIRED)
pkg_check_modules(SERVED_PKG REQUIRED IMPORTED_TARGET served)


target_link_libraries(Backend PkgConfig::SERVED_PKG)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")