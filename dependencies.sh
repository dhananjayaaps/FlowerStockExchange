#!/bin/bash

# Install CMake
CMAKE_VERSION=3.21.2
CMAKE_DIR="/path/to/cmake-${CMAKE_VERSION}-Linux-x86_64/bin"
export PATH="$CMAKE_DIR:$PATH"

# Install MySQL Connector/C++
MYSQLCPPCONN_VERSION=8.0.26
MYSQLCPPCONN_INCLUDE_DIR="/path/to/mysql-connector-c++-${MYSQLCPPCONN_VERSION}-linux-glibc2.12-x86_64/include"
MYSQLCPPCONN_LIB_DIR="/path/to/mysql-connector-c++-${MYSQLCPPCONN_VERSION}-linux-glibc2.12-x86_64/lib"

# Install OpenSSL
# The installation method may vary depending on your operating system

# Windows: Download OpenSSL binaries from https://slproweb.com/products/Win32OpenSSL.html
# macOS: Use Homebrew package manager
# Linux: Use your distribution's package manager

# Example for Linux (Ubuntu):
sudo apt-get install libssl-dev

# Install Boost
# The installation method may vary depending on your operating system

# Windows: Download Boost binaries from https://www.boost.org/users/download/
# macOS: Use Homebrew package manager
# Linux: Use your distribution's package manager

# Example for Linux (Ubuntu):
sudo apt-get install libboost-all-dev

# Install PkgConfig
# The installation method may vary depending on your operating system

# Windows: Download PkgConfig from http://www.gtk.org/download/win64.php
# macOS: Use Homebrew package manager
# Linux: Use your distribution's package manager

# Example for Linux (Ubuntu):
sudo apt-get install pkg-config

# Build and install Served library
SERVED_VERSION=0.6.0
SERVED_DIR="/path/to/served-${SERVED_VERSION}"

# Download served library source code
git clone https://github.com/meltwater/served.git "$SERVED_DIR"
cd "$SERVED_DIR"

# Build and install the served library
mkdir build
cd build
cmake ..
make
sudo make install

