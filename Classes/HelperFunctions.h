#ifndef __HELPER_FUNCTIONS_H__
#define __HELPER_FUNCTIONS_H__

#include "cocos2d.h"

#include <sstream>
#include <string>
#include <random>

template <typename T>
std::string ToString(T t)
{
    std::stringstream s;
    s << t;
    return s.str();
}

float GetRand01();

int   GetRand1N(int n);

#endif //__HELPER_FUNCTIONS_H__
