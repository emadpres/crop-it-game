#ifndef __GAME_CLASSIC_H__
#define __GAME_CLASSIC_H__

#include "cocos2d.h"

class Ball;
class Polygon;

enum class CropperImage
{
    LINE,
    ANGLE
};

class GameClassic : public cocos2d::LayerColor {
public:
    virtual bool init();

    CREATE_FUNC(GameClassic);

private:
    Ball *_ball;
    Polygon *_polygon;
    cocos2d::DrawNode *_drawingNode;


    void RenderPolygon();

    void InitialGameArea();

    cocos2d::Vec2 GetGameAreaCenter();
    cocos2d::Rect GetGameAreaRect();
    cocos2d::Vec2 GetCropperOriginalPos();

    void InitialBall();

    int GetGameAreaSquareWidth();

    void BreakSegment(std::list<std::pair<cocos2d::Vec2, cocos2d::Vec2>>::iterator it, float breakRatio);

    void InitCropper();

    void Crop(cocos2d::Vec2 pos, int dir);
    int Crop(cocos2d::Vec2 star, cocos2d::Vec2 end);

    CropperImage GetInitialDirection() const;

    void SetCropper();

    cocos2d::Sprite *_cropper;
};


#endif //__GAME_CLASSIC_H__
