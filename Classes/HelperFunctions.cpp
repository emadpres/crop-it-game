#include "HelperFunctions.h"
#include "cocos2d.h"
#include <list>


USING_NS_CC;
using namespace std;

float GetRand01() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> dis(0, 1);
    return dis(gen);
}

float CalArea(list<pair<Vec2, Vec2>> segments) {

    float area = 0;

    for(pair<Vec2, Vec2> &seg: segments) {
        area += (seg.first.x + seg.second.x) * (seg.first.y - seg.second.y);
    }

    area /= 2.0f;
//    CCLOG("Area: %f", area);

    return area;

}

int GetRand1N(int n)
{
    std::random_device r;
    std::default_random_engine el(r());
    std::uniform_int_distribution<int> uniform_dist(1, n);
    return uniform_dist(el);
}

