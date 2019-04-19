#ifndef __GAME_CLASSIC_H__
#define __GAME_CLASSIC_H__

#include "cocos2d.h"
#include <list>

class GameClassic : public cocos2d::LayerColor
{
public:
    virtual bool init();
    CREATE_FUNC(GameClassic);

private:
    cocos2d::DrawNode *drawingNode;
    std::list<std::pair<cocos2d::Vec2, cocos2d::Vec2>> _segments;
    void CreateShape_InitialSquare();
    void CreateShape_Custom1();

    void RenderSegment();

    void InitialGameArea();
    cocos2d::Vec2 GetGameAreaCenter();
    int GetGameAreaSquareWidth();
};


#endif //__GAME_CLASSIC_H__
