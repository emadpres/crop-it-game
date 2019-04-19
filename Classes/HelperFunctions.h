#ifndef __HELPER_FUNCTIONS_H__
#define __HELPER_FUNCTIONS_H__

#include <sstream>
#include <string>

template <typename T>
std::string ToString(T t)
{
    std::stringstream s;
    s << t;
    return s.str();
}


#endif //__HELPER_FUNCTIONS_H__
