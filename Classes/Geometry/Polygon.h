#ifndef __POLYGON_H__
#define __POLYGON_H__

#include <list>
#include "cocos2d.h"
#include "TransformInfo.h"

using seg_t = std::pair<cocos2d::Vec2, cocos2d::Vec2>;
using segList_t = std::list<std::pair<cocos2d::Vec2, cocos2d::Vec2>>;
using segListIterator_t = segList_t::iterator;


class Polygon {

public:
    Polygon(cocos2d::Vec2 origin, cocos2d::Size area);

    void Initial_Square(cocos2d::Vec2 leftBottom, int width);

    void Initial_Custom1(cocos2d::Vec2 leftBottom, int width);

    const segList_t &GetSegments() const;

    void AddSegment(seg_t seg);

    void Crop(cocos2d::Vec2 pos, int dir, cocos2d::Vec2 ballPos);

    bool IsPointInsidePolygon(cocos2d::Vec2 point) const;

    float CalcArea();

    Polygon &operator=(const Polygon &second);

    TransformInfo* EstimateScaleUp();

private:
    cocos2d::Vec2 _origin;
    cocos2d::Size _area;
    segList_t _segments;

    segListIterator_t Ray(cocos2d::Vec2 startPos, cocos2d::Vec2 dir);

    cocos2d::Vec2 RayPos(cocos2d::Vec2 startPos, cocos2d::Vec2 dir);

    int RayCount(cocos2d::Vec2 startPos, cocos2d::Vec2 dir) const;

    float FindIntersectionPoint(cocos2d::Vec2 a_start, cocos2d::Vec2 a_end, cocos2d::Vec2 b_start, cocos2d::Vec2 b_end) const;

    void FindBoundaryXY(float &x_min, float &x_max, float &y_min, float &y_max);

    cocos2d::Vec2 BreakSegment(segListIterator_t it, float breakRatio);

    void SplitPolygon(cocos2d::Vec2 &pos, const segListIterator_t &it1, const segListIterator_t &it2,
                      cocos2d::Vec2 &rayCollisionPoint1, cocos2d::Vec2 &rayCollisionPoint2, Polygon *&poly1,
                      Polygon *&poly2) const;
};


#endif //__POLYGON_H__
