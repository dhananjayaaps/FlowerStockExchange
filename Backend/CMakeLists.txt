cmake_minimum_required(VERSION 3.24)
project(Backend)

set(CMAKE_CXX_STANDARD 17)

add_executable(Backend main.cpp processOrder.cpp order.cpp RedBlackTree.h BuyHandler.cpp RedBlackTree.cpp)
