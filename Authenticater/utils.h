#ifndef AUTHENTICATER_UTILS_H
#define AUTHENTICATER_UTILS_H

#include <iostream>
#include <random>
#include <string>
#include <openssl/sha.h>

std::string generateToken(int length = 10);
bool isEmailValid(const std::string& email);
std::string sha256Hash(const std::string& password);
std::string decodeEmail(const std::string& encodedEmail);
bool validateSHA256Hash(const std::string& password, const std::string& hashedPassword);

#endif //AUTHENTICATER_UTILS_H
