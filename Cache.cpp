#include <chrono>
#include <iomanip>
#include "Cache.h"

int count = 0;
std::string currentTime;

void incrementCount() {
    count++;  // Increment the count by 1
}

int getCount() {
    return count;  // Return the current count value
}

void setTime() {
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm local_time = *std::localtime(&now_c);
    std::ostringstream oss;
    oss << std::put_time(&local_time, "%Y-%m-%d %H:%M:%S");
    oss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    currentTime = oss.str(); //set the local time
}

std::string getTime() {
    return currentTime;
}






