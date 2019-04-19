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

float CalArea(std::list<std::pair<cocos2d::Vec2, cocos2d::Vec2>> segments);

#endif //__HELPER_FUNCTIONS_H__
