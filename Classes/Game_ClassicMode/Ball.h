#ifndef __BALL_H
#define __BALL_H

#include "cocos2d.h"
#include "string"
#include "Geometry/GeometryTypes.h"

class Ball : public cocos2d::Node {
public:
    static Ball *create(const std::string &filename, const segList_t *segments, const seg_t *cropperLine1, const seg_t *cropperLine2);

    Ball(const std::string &filename, const segList_t *segments, const seg_t *cropperLine1, const seg_t *cropperLine2);

    cocos2d::Vec2 GetVelocity() const { return _velocity; }

    cocos2d::Vec2 EstimateMove(float deltaTime) const;

    void SetVelocity(cocos2d::Vec2 v);

    bool MoveBall(float dt);

    void SetCropperLinesValidity(bool newStatus);

private:
    cocos2d::Vec2 CalcBallPossibleCollisionPos(const seg_t &line);
private:
    cocos2d::Vec2 _velocity;
    cocos2d::Sprite *_ballSprite;

    const segList_t *_segments;
    bool isCropperLineValid;
    const seg_t *_cropperLine1,*_cropperLine2;
};


#endif //__BALL_H
