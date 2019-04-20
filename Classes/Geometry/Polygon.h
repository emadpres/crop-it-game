#ifndef __POLYGON_H__
#define __POLYGON_H__

#include <list>
#include "cocos2d.h"

using seg_t = std::pair<cocos2d::Vec2, cocos2d::Vec2>;
using segList_t = std::list<std::pair<cocos2d::Vec2, cocos2d::Vec2>>;
using segListIterator_t = segList_t::iterator;

class Polygon {

public:
    Polygon();

    void Initial_Square(cocos2d::Vec2 leftBottom, int width);

    void Initial_Custom1(cocos2d::Vec2 leftBottom, int width);

    const segList_t &GetSegments() const;

    void AddSegment(seg_t seg);

    void Crop(cocos2d::Vec2 pos, int dir, cocos2d::Vec2 ballPos);

    bool IsPointInsidePolygon(cocos2d::Vec2 point) const;

    Polygon &operator=(const Polygon &second);

private:
    segList_t _segments;

    segListIterator_t Ray(cocos2d::Vec2 startPos, cocos2d::Vec2 dir);

    int RayCount(cocos2d::Vec2 startPos, cocos2d::Vec2 dir) const;

    float CalcCutPoint(cocos2d::Vec2 a_start, cocos2d::Vec2 a_end, cocos2d::Vec2 b_start, cocos2d::Vec2 b_end) const;

    cocos2d::Vec2 BreakSegment(segListIterator_t it, float breakRatio);

    void SplitPolygon(cocos2d::Vec2 &pos, const segListIterator_t &it1, const segListIterator_t &it2,
                      cocos2d::Vec2 &rayCollisionPoint1, cocos2d::Vec2 &rayCollisionPoint2, Polygon *&poly1,
                      Polygon *&poly2) const;
};


#endif //__POLYGON_H__
