#include "Ball.h"
#include "HelperFunctions.h"
USING_NS_CC;
using namespace std;

Ball::Ball(const std::string &filename, const segList_t *segments, const seg_t *cropperLine1, const seg_t *cropperLine2)
        : _velocity(40, 40), _segments(segments),
        isCropperLineValid(false), _cropperLine1(cropperLine1), _cropperLine2(cropperLine2)  {

    setAnchorPoint(Vec2::ZERO);
    _ballSprite = Sprite::create(filename);
    _ballSprite->setPosition(Vec2::ZERO);
    addChild(_ballSprite);
    setContentSize(_ballSprite->getContentSize());
}


Ball *Ball::create(const std::string &filename, const segList_t *segments, const seg_t *cropperLine1, const seg_t *cropperLine2) {
    Ball *ball = new(std::nothrow) Ball(filename, segments, cropperLine1, cropperLine2);
    if (ball) {
        ball->autorelease();
        return ball;
    }
    CC_SAFE_DELETE(ball);
    return nullptr;
}


cocos2d::Vec2 Ball::EstimateMove(float deltaTime) const {
    Vec2 cur = getPosition();
    Vec2 next = cur + deltaTime * GetVelocity();
    return next;
}

void Ball::SetCropperLinesValidity(bool newStatus)
{
    isCropperLineValid = newStatus;
}

void Ball::SetVelocity(Vec2 v) {
    _velocity = v;
}


Vec2 Ball::CalcBallPossibleCollisionPos(const seg_t &line)
{
    Vec2 ret, currentPos=getPosition();

    if (line.first.x == line.second.x) { // Vertical Line
        if (_velocity.x > 0) {
            ret.x = currentPos.x + getContentSize().width / 2;
            ret.y = currentPos.y;
        } else {
            ret.x = currentPos.x - getContentSize().width / 2;
            ret.y = currentPos.y;
        }
    } else {
        if (_velocity.y > 0) {
            ret.x = currentPos.x;
            ret.y = currentPos.y + getContentSize().height / 2;
        } else {
            ret.x = currentPos.x;
            ret.y = currentPos.y - getContentSize().height / 2;
        }
    }
    return ret;
}

bool Ball::MoveBall(float dt) {
    float s, t;

    auto currentPos = getPosition();
    Vec2 pos;

    Vec2 newPos = EstimateMove(dt);

    if(isCropperLineValid)
    {
        pos = CalcBallPossibleCollisionPos(*_cropperLine1);
        if (Vec2::isLineIntersect(pos, newPos, _cropperLine1->first, _cropperLine1->second, &s, &t) &&
            (s >= 0 && s <= 1 && t >= 0 && t <= 1)) {
            return true;
        }

        pos = CalcBallPossibleCollisionPos(*_cropperLine2);
        if (Vec2::isLineIntersect(pos, newPos, _cropperLine2->first, _cropperLine2->second, &s, &t) &&
            (s >= 0 && s <= 1 && t >= 0 && t <= 1)) {
            return true;
        }
    }

    for (auto &seg : *_segments) {
//        if (seg.first.x == seg.second.x) {
//            if (_velocity.x > 0) {
//                pos.x = currentPos.x + getContentSize().width / 2;
//                pos.y = currentPos.y;
//            } else {
//                pos.x = currentPos.x - getContentSize().width / 2;
//                pos.y = currentPos.y;
//            }
//        } else {
//            if (_velocity.y > 0) {
//                pos.x = currentPos.x;
//                pos.y = currentPos.y + getContentSize().height / 2;
//            } else {
//                pos.x = currentPos.x;
//                pos.y = currentPos.y - getContentSize().height / 2;
//            }
//        }
        pos = CalcBallPossibleCollisionPos(seg);

        //currentPos
        if (Vec2::isLineIntersect(pos, newPos, seg.first, seg.second, &s, &t) &&
            (s >= 0 && s <= 1 && t >= 0 && t <= 1)) {
            //auto degree = atan((newPos.y - currentPos.y) / (newPos.x - currentPos.x));
            if (abs(seg.first.x - seg.second.x) < 0.1)
                _velocity.x *= -1.0f;
            else if (abs(seg.first.y - seg.second.y) < 0.1)
                _velocity.y *= -1.0f;
            else
            CCLOG("INTERSECT!!!");
//            auto marker = Sprite::create("marker.png");
//            marker->setScale(10.0f);
//            marker->setPosition(Vec2(seg.first.x + t * (seg.second.x - seg.first.x),
//                                     seg.first.y + t * (seg.second.y - seg.first.y)));
//            this->getParent()->addChild(marker);
            break;
        }
    }
    newPos = EstimateMove(dt);
    setPosition(newPos);
    return false;
}

