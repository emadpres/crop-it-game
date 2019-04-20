#ifndef __MAINMENU_LAYER_H__
#define __MAINMENU_LAYER_H__

#include "cocos2d.h"

class MainMenu : public cocos2d::LayerColor
{
public:
    static cocos2d::Scene* createScene(bool isMainMenu);
    static MainMenu* create(bool isMainScene);

protected:
    virtual bool init(bool isMainMenu);
};

#endif // __MAINMENU_LAYER_H__
