#ifndef __BALL_H
#define __BALL_H

#include "cocos2d.h"
#include "string"

class Ball : public cocos2d::Node {
public:
    static Ball *create(const std::string &filename,
            const std::list<std::pair<cocos2d::Vec2, cocos2d::Vec2>> * segments);

    Ball(const std::string &filename,
         const std::list<std::pair<cocos2d::Vec2, cocos2d::Vec2>> * segments);

    cocos2d::Vec2 GetVelocity() const { return _velocity; }

    cocos2d::Vec2 EstimateMove(float deltaTime) const;

    void SetVelocity(cocos2d::Vec2 v);

    void MoveBall(float dt);

private:
    cocos2d::Vec2 _velocity;
    cocos2d::Sprite *_ballSprite;

    const std::list<std::pair<cocos2d::Vec2, cocos2d::Vec2>> * _segments;
};


#endif //__BALL_H
