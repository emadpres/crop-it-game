//
// Created by Emad Aghajani on 2019-04-20.
//

#ifndef __TRANSFORMINFO_H__
#define __TRANSFORMINFO_H__

#include "cocos2d.h"

class TransformInfo {

public:
    cocos2d::Vec2 origin;
    float x_min, y_min;
    float w, h;
    float scale;
    float animationProgress01 = 0;
};


#endif //__TRANSFORMINFO_H__
