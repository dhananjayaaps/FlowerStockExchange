#ifndef BACKEND_CACHE_H
#define BACKEND_CACHE_H

#include <iostream>

// Counter.h

extern int count;
extern std::string currentTime;

void incrementCount();
int getCount();

void setTime();
std::string getTime();


#endif //BACKEND_CACHE_H
