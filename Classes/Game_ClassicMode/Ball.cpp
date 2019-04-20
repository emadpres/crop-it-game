#include "Ball.h"
#include "HelperFunctions.h"

USING_NS_CC;
using namespace std;

Ball::Ball(const std::string &filename,
           const std::list<std::pair<cocos2d::Vec2, cocos2d::Vec2>> * segments)
           : _velocity(40, 40), _segments(segments) {
    setAnchorPoint(Vec2::ZERO);

    _ballSprite = Sprite::create(filename);
    _ballSprite->setPosition(Vec2::ZERO);
    addChild(_ballSprite);
    setContentSize(_ballSprite->getContentSize());

//    auto marker= Sprite::create("marker.png");
//    marker->setColor(Color3B::RED);
//    marker->setScale(10);
//    marker->setPosition(Vec2::ZERO);
//    addChild(marker,100);
}


Ball *Ball::create(const std::string &filename,
                   const std::list<std::pair<cocos2d::Vec2, cocos2d::Vec2>> * segments) {
    Ball *ball = new(std::nothrow) Ball(filename, segments);
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

void Ball::SetVelocity(Vec2 v) {
    _velocity = v;
}

void Ball::MoveBall(float dt)
{
    float s, t;

    auto currentPos = getPosition();
    Vec2 pos;

    Vec2 newPos = EstimateMove(dt);
    for (auto& seg : *_segments)
    {
        if (seg.first.x == seg.second.x)
        {
            if (_velocity.x > 0)
            {
                pos.x = currentPos.x + getContentSize().width / 2;
                pos.y = currentPos.y;
            }
            else
            {
                pos.x = currentPos.x - getContentSize().width / 2;
                pos.y = currentPos.y;
            }
        } else
        {
            if (_velocity.y > 0)
            {
                pos.x = currentPos.x;
                pos.y = currentPos.y + getContentSize().height / 2;
            }
            else
            {
                pos.x = currentPos.x;
                pos.y = currentPos.y - getContentSize().height / 2;
            }
        }

        //currentPos
        if (Vec2::isLineIntersect(pos, newPos, seg.first, seg.second, &s, &t) &&
                (s >=0 && s <= 1 && t >= 0 && t <= 1))
        {
            //auto degree = atan((newPos.y - currentPos.y) / (newPos.x - currentPos.x));
            if (seg.first.x == seg.second.x)
                _velocity.x *= -1.0f;
            else if (seg.first.y == seg.second.y)
                _velocity.y *= -1.0f;

            //CCLOG("INTERSECT!!!");
            break;
        }
    }
    newPos = EstimateMove(dt);
    setPosition(newPos);
}

