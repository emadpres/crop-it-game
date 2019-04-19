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

//    list<pair<Vec2, Vec2>> _segments;

//    example1
//    _segments.push_back(make_pair(Vec2(-1,1), Vec2(1,1)));
//    _segments.push_back(make_pair(Vec2(1,1), Vec2(1,-1)));
//    _segments.push_back(make_pair(Vec2(1,-1), Vec2(-1,-1)));
//    _segments.push_back(make_pair(Vec2(-1,-1), Vec2(-1,1)));

//    example2
//    _segments.push_back(make_pair(Vec2(4,6), Vec2(4,-4)));
//    _segments.push_back(make_pair(Vec2(4,-4), Vec2(8,-4)));
//    _segments.push_back(make_pair(Vec2(8,-4), Vec2(8,-8)));
//    _segments.push_back(make_pair(Vec2(8,-8), Vec2(-4,-8)));
//    _segments.push_back(make_pair(Vec2(-4,-8), Vec2(-4,6)));
//    _segments.push_back(make_pair(Vec2(-4,6), Vec2(4,6)));

    float area = 0;

    for(pair<Vec2, Vec2> &seg: segments) {
        area += (seg.first.x + seg.second.x) * (seg.first.y - seg.second.y);
    }

    area /= 2;
    CCLOG("Area: %f", area);

    return area;

}