#ifndef __POLYGON_H__
#define __POLYGON_H__

#include <list>
#include "cocos2d.h"


class Polygon {

public:
    Polygon();

    const std::list<std::pair<cocos2d::Vec2, cocos2d::Vec2>> &GetSegments() const;

    void AddSegment(std::pair<cocos2d::Vec2, cocos2d::Vec2> seg);


    void BreakSegment(std::list<std::pair<cocos2d::Vec2, cocos2d::Vec2>>::iterator it, float breakRatio);
    void Crop(cocos2d::Vec2 pos, int dir);
    int Crop(cocos2d::Vec2 start, cocos2d::Vec2 end);

    void Initial_Square(cocos2d::Vec2 leftBottom, int width);
    void Initial_Custom1(cocos2d::Vec2 leftBottom, int width);

private:
    std::list<std::pair<cocos2d::Vec2, cocos2d::Vec2>> _segments;
};


#endif //__POLYGON_H__
