#ifndef __BALL_H
#define __BALL_H

#include "cocos2d.h"
#include "string"

class Ball : public cocos2d::Node {
public:
    static Ball *create(const std::string &filename);

    Ball(const std::string &filename);

    cocos2d::Vec2 GetVelocity() const { return _velocity; }

    cocos2d::Vec2 EstimateMove(float deltaTime) const;

private:
    cocos2d::Vec2 _velocity;
    cocos2d::Sprite *_ballSprite;
};


#endif //__BALL_H
