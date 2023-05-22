#include "utils.h"
#include <random>
#include <string>
#include <sstream>
#include <iomanip>
#include <openssl/evp.h>
#include <boost/algorithm/string/replace.hpp>

std::string generateToken(int length) {
    // Characters to include in the token
    const std::string characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

    // Random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, characters.size() - 1);

    // Generate the token
    std::string token;
    for (int i = 0; i < length; ++i) {
        token += characters[dis(gen)];
    }

    return token;
}


bool isChar(char c)
{
    return ((c >= 'a' && c <= 'z')
            || (c >= 'A' && c <= 'Z'));
}

std::string sha256Hash(const std::string& password) {
    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    EVP_DigestInit(mdctx, EVP_sha256());

    // Hash the password
    EVP_DigestUpdate(mdctx, password.c_str(), password.length());

    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hashLen;

    // Finalize the hash
    EVP_DigestFinal(mdctx, hash, &hashLen);

    EVP_MD_CTX_free(mdctx);

    std::stringstream ss;
    for (unsigned int i = 0; i < hashLen; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    return ss.str();
}

bool isEmailValid(const std::string& email)
{
    // Check the first character
    // is an alphabet or not
    if (!isChar(email[0])) {

        // If it's not an alphabet
        // email id is not valid
        return 0;
    }
    // Variable to store position
    // of At and Dot
    int At = -1, Dot = -1;

    // Traverse over the email id
    // string to find position of
    // Dot and At
    for (int i = 0;
         i < email.length(); i++) {

        // If the character is '@'
        if (email[i] == '@') {

            At = i;
        }

            // If character is '.'
        else if (email[i] == '.') {

            Dot = i;
        }
    }

    // If At or Dot is not present
    if (At == -1 || Dot == -1)
        return 0;

    // If Dot is present before At
    if (At > Dot)
        return 0;

    // If Dot is present at the end
    return !(Dot >= (email.length() - 1));
}


bool validateSHA256Hash(const std::string& password, const std::string& hashedPassword) {
    std::string inputHash = sha256Hash(password);
    return (inputHash == hashedPassword);
}

std::string decodeEmail(const std::string& encodedEmail) {
    std::string decodedEmail = encodedEmail;
    boost::algorithm::replace_all(decodedEmail, "%40", "@");
    return decodedEmail;
}