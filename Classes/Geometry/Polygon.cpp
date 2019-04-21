#include "Polygon.h"
#include "HelperFunctions.h"
#include "Geometry/TransformInfo.h"
USING_NS_CC;
using namespace std;


Polygon::Polygon(Vec2 origin, Size area):_origin(origin), _area(area)  {
}

Polygon::Polygon(const Polygon &p){
    this->_origin = p._origin;
    this->_area = p._area;

    for (const auto &seg: p._segments)
        AddSegment(seg);
}

Polygon &Polygon::operator=(const Polygon &second) {

    this->_origin = second._origin;
    this->_area = second._area;

    _segments.clear();
    for (const auto &seg: second._segments)
        AddSegment(seg);

    return (*this);
}

void Polygon::AddSegment(seg_t seg) {
    _segments.push_back(seg);
}

const segList_t &Polygon::GetSegments() const {
    return _segments;
}

segList_t &Polygon::GetSegments() {
    return _segments;
}

void Polygon::Initial_Square(Vec2 leftBottom, int width) {
    const auto center = leftBottom + Vec2(width / 2, width / 2);
    _segments.clear();
    AddSegment(make_pair(center + Vec2(-width / 2, +width / 2), center + Vec2(+width / 2, +width / 2)));
    AddSegment(make_pair(center + Vec2(+width / 2, +width / 2), center + Vec2(+width / 2, -width / 2)));
    AddSegment(make_pair(center + Vec2(+width / 2, -width / 2), center + Vec2(-width / 2, -width / 2)));
    AddSegment(make_pair(center + Vec2(-width / 2, -width / 2), center + Vec2(-width / 2, +width / 2)));
}

void Polygon::Initial_Custom1(Vec2 leftBottom, int width) {
    const auto center = leftBottom + Vec2(width / 2, width / 2);
    _segments.clear();
    AddSegment(make_pair(center + Vec2(-width / 2, +width / 2), center + Vec2(+width / 2, +width / 2)));
    AddSegment(make_pair(center + Vec2(+width / 2, +width / 2), center + Vec2(+width / 2, -width / 2)));
    AddSegment(make_pair(center + Vec2(+width / 2, -width / 2), center + Vec2(0, -width / 2)));
    AddSegment(make_pair(center + Vec2(0, -width / 2), center + Vec2(0, 0)));
    AddSegment(make_pair(center + Vec2(0, 0), center + Vec2(-width / 2, 0)));
    AddSegment(make_pair(center + Vec2(-width / 2, 0), center + Vec2(-width / 2, +width / 2)));
}

int Polygon::RayCount(Vec2 startPos, Vec2 dir) const {
    int count = 0;
    float s, t;
    auto it = _segments.begin();
    while (it != _segments.end()) {
        if (Vec2::isLineIntersect(startPos, startPos + dir * 1000, (*it).first, (*it).second, &s, &t)) {
            if (s >= 0 && t >= 0 && s <= 1 && t <= 1) {
                count++;
            }
        }
        it++;
    }
    return count;
}

Vec2 Polygon::RayPos(Vec2 startPos, Vec2 dir)
{
    segListIterator_t it = Ray(startPos, dir);
    if (it!=_segments.end()) {
        float r = FindIntersectionPoint(startPos, startPos + dir * 1000, it->first, it->second);
        return Vec2(it->first.x+r*(it->second.x-it->first.x),
                    it->first.y+r*(it->second.y-it->first.y));
    } else
        return Vec2::ZERO;
}


segListIterator_t Polygon::Ray(Vec2 startPos, Vec2 dir) {
    segListIterator_t nearestSeg_it = _segments.end();
    float nearestSeg_s = 1.1f;
    auto it = _segments.begin();
    float s, t;
    while (it != _segments.end()) {
        if (Vec2::isLineIntersect(startPos, startPos + dir * 1000, (*it).first, (*it).second, &s, &t)) {
            if (s >= 0 && t >= 0 && s <= 1 && t <= 1) {
                if (s < nearestSeg_s) {
                    nearestSeg_it = it;
                    nearestSeg_s = s;
                }
            }

        }
        it++;
    }
    return nearestSeg_it;
}

Vec2 Polygon::BreakSegment(segListIterator_t it, float breakRatio) {
    Vec2 start = (*it).first;
    Vec2 end = (*it).second;
    it->second.x = start.x + (end.x - start.x) * breakRatio;
    it->second.y = start.y + (end.y - start.y) * breakRatio;
    auto firstPartEnd = (*it).second;
    it++;
    _segments.insert(it, make_pair(firstPartEnd, end));
    //CCLOG("New number of segments: %zu", _segments.size());

    return firstPartEnd;
}

/*
 * Returns t, the ration on the second line: intersection.x = b_start.x+t*(b_end.x-b_start.x)
 */
float Polygon::FindIntersectionPoint(Vec2 a_start, Vec2 a_end, Vec2 b_start, Vec2 b_end) const {
    float s, t;
    if (Vec2::isLineIntersect(a_start, a_end, b_start, b_end, &s, &t)) {
        if (s >= 0 && t >= 0 && s <= 1 && t <= 1) {
            return t;
        }
    }
    return 0;
}

void Polygon::Crop(Vec2 pos, int dir, Vec2 ballPos, Vec2 &removedSideCenter) {

    if (false == IsPointInsidePolygon(pos))
        return;

    Vec2 dir1, dir2;
    TranslateDir(dir, dir1, dir2);


    segListIterator_t it1, it2;
    Vec2 rayCollisionPoint1, rayCollisionPoint2;


    it1 = Ray(pos, dir1);
    float r = FindIntersectionPoint(pos, pos + dir1 * 1000, it1->first, it1->second);
    rayCollisionPoint1 = BreakSegment(it1, r);

    it2 = Ray(pos, dir2);
    r = FindIntersectionPoint(pos, pos + dir2 * 1000, it2->first, it2->second);
    rayCollisionPoint2 = BreakSegment(it2, r);

    Polygon *poly1;
    Polygon *poly2;
    SplitPolygon(pos, it1, it2, rayCollisionPoint1, rayCollisionPoint2, poly1, poly2);


    if (poly1->IsPointInsidePolygon(ballPos)) {
        *this = *poly1;
        removedSideCenter = poly2->GetCenter();
    }
    else {
        *this = *poly2;
        removedSideCenter = poly1->GetCenter();
    }

    delete poly1;
    delete poly2;
}

cocos2d::Vec2 Polygon::GetCenter()
{
    Vec2 center = Vec2::ZERO;
    for (const auto &seg: _segments)
        center += seg.first;
    center = center/_segments.size();
    return center;
}

void Polygon::SplitPolygon(Vec2 &pos, const segListIterator_t &it1, const segListIterator_t &it2, Vec2 &rayCollisionPoint1,
                      Vec2 &rayCollisionPoint2, Polygon *&poly1, Polygon *&poly2) const {
    poly1 = new Polygon(_origin, _area);
    poly2 = new Polygon(_origin, _area);
    bool reachedFirstBreakingPoint = false;
    Polygon **activePoly = &poly1;

    auto currentPolyIt = _segments.begin();
    while (currentPolyIt != _segments.end()) {
        if (currentPolyIt == it1 || currentPolyIt == it2) {
            if (false == reachedFirstBreakingPoint) {
                reachedFirstBreakingPoint = true;
                (*activePoly)->AddSegment(make_pair(currentPolyIt->first, currentPolyIt->second));
                if (currentPolyIt == it1) {
                    (*activePoly)->AddSegment(make_pair(rayCollisionPoint1, pos));
                    (*activePoly)->AddSegment(make_pair(pos, rayCollisionPoint2));
                } else {
                    (*activePoly)->AddSegment(make_pair(rayCollisionPoint2, pos));
                    (*activePoly)->AddSegment(make_pair(pos, rayCollisionPoint1));
                }
                activePoly = (activePoly == &poly1) ? (&poly2) : (&poly1);
                if (currentPolyIt == it1) {
                    (*activePoly)->AddSegment(make_pair(rayCollisionPoint2, pos));
                    (*activePoly)->AddSegment(make_pair(pos, rayCollisionPoint1));
                } else {
                    (*activePoly)->AddSegment(make_pair(rayCollisionPoint1, pos));
                    (*activePoly)->AddSegment(make_pair(pos, rayCollisionPoint2));
                }
            } else {
                (*activePoly)->AddSegment(make_pair(currentPolyIt->first, currentPolyIt->second));
                activePoly = (activePoly == &poly1) ? (&poly2) : (&poly1);
            }
        } else
            (*activePoly)->AddSegment(make_pair(currentPolyIt->first, currentPolyIt->second));
        currentPolyIt++;

    }
}

bool Polygon::IsPointInsidePolygon(Vec2 point) const {
    int c = RayCount(point, Vec2(1, 0));
    return (c % 2 == 1);
}




float Polygon::CalcArea() {

    float area = 0;
    for (const auto &seg: _segments)
        area += (seg.first.x + seg.second.x) * (seg.first.y - seg.second.y);
    area /= 2.0;
    return area;
}


void Polygon::ScaleUp(TransformInfo *ti)
{

    for (auto &seg: _segments) {

        ti->TranformVec2(seg.first);
        ti->TranformVec2(seg.second);
    }

}

TransformInfo* Polygon::EstimateScaleUp()
{
    float x_min, x_max, y_min, y_max;
    FindBoundaryXY(x_min, x_max, y_min, y_max);

    float polyWidth = x_max - x_min;
    float wScale = _area.width/polyWidth;
    int polyheight = y_max - y_min;
    float hScale = _area.height/polyheight;

    float scaleFactor = min(wScale, hScale);

    auto t = new TransformInfo(_origin, x_min, y_min, x_max-x_min, y_max-y_min, scaleFactor,_area);

    return t;
}

void Polygon::FindBoundaryXY(float &x_min, float &x_max, float &y_min, float &y_max)
{
    x_min = numeric_limits<float>::max();
    y_min = numeric_limits<float>::max();

    x_max = -1*numeric_limits<float>::min();
    y_max = -1*numeric_limits<float>::max();

    for (const auto &seg: _segments) {
        // seg.first
        if (seg.first.x < x_min)
            x_min = seg.first.x;
        if (seg.first.x > x_max)
            x_max = seg.first.x;

        if (seg.first.y < y_min)
            y_min = seg.first.y;
        if (seg.first.y > y_max)
            y_max = seg.first.y;

        // seg.second
        if (seg.second.x < x_min)
            x_min = seg.second.x;
        if (seg.second.x > x_max)
            x_max = seg.second.x;

        if (seg.second.y < y_min)
            y_min = seg.second.y;
        if (seg.second.y > y_max)
            y_max = seg.second.y;
    }
}