#include "Polygon.h"

USING_NS_CC;
using namespace std;


Polygon::Polygon() {
}

void Polygon::AddSegment(seg_t seg) {
    _segments.push_back(seg);
}

const segList_t &Polygon::GetSegments() const {
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
    CCLOG("New number of segments: %zu", _segments.size());

    return firstPartEnd;
}

float Polygon::CalcCutPoint(Vec2 a_start, Vec2 a_end, Vec2 b_start, Vec2 b_end) const {
    float s, t;
    if (Vec2::isLineIntersect(a_start, a_end, b_start, b_end, &s, &t)) {
        if (s >= 0 && t >= 0 && s <= 1 && t <= 1) {
            return t;
        }
    }
    return 0;
}

void Polygon::Crop(Vec2 pos, int dir, Vec2 ballPos) {

    if(false==IsPointInsidePolygon(pos))
        return;

    Vec2 dir1, dir2;
    switch(dir)
    {
        case 1: // Right,Down
            dir1 = Vec2(1, 0);
            dir2 = Vec2(0, -1);
            break;
        case 2:
            dir1 = Vec2(0, -1);
            dir2 = Vec2(-1, 0);
            break;
        case 3:
            dir1 = Vec2(-1, 0);
            dir2 = Vec2(0, 1);
            break;
        case 4:
            dir1 = Vec2(0, 1);
            dir2 = Vec2(1, 0);
            break;
        case 5:
            dir1 = Vec2(0, 1);
            dir2 = Vec2(0, -1);
            break;
        case 6:
        default:
            dir1 = Vec2(1, 0);
            dir2 = Vec2(-1, 0);
            break;
    }


    segListIterator_t it1, it2;
    Vec2 rayCollisionPoint1, rayCollisionPoint2;


    it1 = Ray(pos, dir1);
    float r = CalcCutPoint(pos, pos + dir1 * 1000, it1->first, it1->second);
    rayCollisionPoint1 = BreakSegment(it1, r);

    it2 = Ray(pos, dir2);
    r = CalcCutPoint(pos, pos + dir2 * 1000, it2->first, it2->second);
    rayCollisionPoint2 = BreakSegment(it2, r);

    Polygon *poly1;
    Polygon *poly2;

    SplitPolygon(pos, it1, it2, rayCollisionPoint1, rayCollisionPoint2, poly1, poly2);

    if (poly1->IsPointInsidePolygon(ballPos))
        *this = *poly1;
    else
        *this = *poly2;
}

void
Polygon::SplitPolygon(Vec2 &pos, const segListIterator_t &it1, const segListIterator_t &it2, Vec2 &rayCollisionPoint1,
                      Vec2 &rayCollisionPoint2, Polygon *&poly1, Polygon *&poly2) const {
    poly1 = new Polygon();
    poly2 = new Polygon();
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


Polygon &Polygon::operator=(const Polygon &second) {
    _segments.clear();
    for (const auto &seg: second._segments)
        AddSegment(seg);

    return (*this);
}
