#ifndef __GAME_CLASSIC_H__
#define __GAME_CLASSIC_H__

#include "cocos2d.h"
#include "Geometry/TransformInfo.h"

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
    cocos2d::DrawNode *_drawing_bg;
    cocos2d::DrawNode *_drawing_poly;

    void RenderPolygon();

    void InitialGameArea();

    cocos2d::Vec2 GetGameAreaCenter();
    cocos2d::Rect GetGameAreaRect();
    cocos2d::Vec2 GetCropperOriginalPos();

    void InitialBall();

    int GetGameAreaSquareWidth();

    void InitCropper();

    CropperImage GetInitialDirection() const;

    void SetCropper();

    cocos2d::Sprite *_cropper;
    int cropperDir;
    bool isCropperLinesValid;
    cocos2d::Node *_arrows;
    bool _isRotatable;
    cocos2d::Label *_tapLabel;

    void ScaleUpAnimationRunner(float dt);
    void CropperLineAnimationRunner(float dt);
    TransformInfo* _polyTransformInfo;
    Polygon *_targetPolyAfterAnimation;
    cocos2d::Vec2 _targetBallPos;


    cocos2d::Vec2 _cropperDropPoint;
    cocos2d::Vec2 _cropperLineIntersectionPointWithPoly1, _cropperLineIntersectionPointWithPoly2;
    cocos2d::Vec2 _cropperLineEnd1, _cropperLineEnd2;

    void IntialBallMovement();

    void UpdateHud(int percentage);

    cocos2d::Label *_levelLabel;
    cocos2d::ProgressTimer *_progressBar;
};


#endif //__GAME_CLASSIC_H__
