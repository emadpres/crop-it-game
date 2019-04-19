#include "Ball.h"

USING_NS_CC;
using namespace std;

Ball::Ball(const std::string &filename) : _velocity(40, 40) {
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


Ball *Ball::create(const std::string &filename) {
    Ball *ball = new(std::nothrow) Ball(filename);
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

