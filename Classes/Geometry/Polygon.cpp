#include "Polygon.h"

USING_NS_CC;
using namespace std;


Polygon::Polygon() {
}

void Polygon::AddSegment(pair<Vec2, Vec2> seg) {
    _segments.push_back(seg);
}

const list<pair<Vec2, Vec2>> &Polygon::GetSegments() const {
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

segIterator Polygon::Ray(Vec2 startPos, Vec2 dir) {
    segIterator nearestSeg_it = _segments.end();
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

void Polygon::BreakSegment(segIterator it, float breakRatio) {
//    list<pair<Vec2, Vec2>>::iterator it = _segments.begin();
//    int cur = 0;
//    while(cur!=segIndex && it!= _segments.end()) {
//        it++;
//        cur++;
//    }
    Vec2 start = (*it).first;
    Vec2 end = (*it).second;
    it->second.x = start.x + (end.x - start.x) * breakRatio;
    it->second.y = start.y + (end.y - start.y) * breakRatio;
    auto updatedEnd = (*it).second;
    it++;
    _segments.insert(it, make_pair(updatedEnd, end));
    CCLOG("New number of segments: %zu", _segments.size());
}

float Polygon::CalcCutPoint(Vec2 a_start, Vec2 a_end, Vec2 b_start, Vec2 b_end) {
    float s, t;
    if (Vec2::isLineIntersect(a_start, a_end, b_start, b_end, &s, &t)) {
        if (s >= 0 && t >= 0 && s <= 1 && t <= 1) {
            return t;
        }
    }
    return 0;
}

void Polygon::Crop(Vec2 pos, int dir) {
    if (dir == 1) // Right,Down
    {
        auto it = Ray(pos, Vec2(1, 0));
        float r = CalcCutPoint(pos, pos + Vec2(1, 0) * 1000, it->first, it->second);
        BreakSegment(it, r);
    }
//    int first = Crop(pos, pos+Vec2(1000,0));
//    int second = Crop(pos, pos+Vec2(0,-1000));
//    if(second<first) {
//        int t = first;
//        first = second;
//        second = t+1;
//    }

//    std::list<std::pair<cocos2d::Vec2, cocos2d::Vec2>> _newSegments;
//    bool keepInnerPart = true;
//    int i =0;
//    for (const auto &seg: _segments)
//    {
//        if(keepInnerPart) {
//            if(i>first && i<=second)
//                _newSegments.push_back(seg);
//
//        }
//        i++;
//    }
//
//    _segments.clear();
//    for (const auto &seg: _newSegments)
//        _segments.push_back(seg);
}

int Polygon::Crop(Vec2 start, Vec2 end) {
    float s, t;
    int breakIndex = 0;
    auto it = _segments.begin();
    while (it != _segments.end()) {
        if (Vec2::isLineIntersect(start, end, (*it).first, (*it).second, &s, &t)) {
            if (s >= 0 && t >= 0 && s <= 1 && t <= 1) {
                BreakSegment(it, t);
                break;
            }

        }
        it++;
        breakIndex++;
    }
    return breakIndex;
}