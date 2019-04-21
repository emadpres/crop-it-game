//
// Created by Emad Aghajani on 2019-04-20.
//

#ifndef __TRANSFORMINFO_H__
#define __TRANSFORMINFO_H__

#include "cocos2d.h"

class TransformInfo {

public:
    float _animationProgress01;

    TransformInfo(cocos2d::Vec2 origin, float x_min, float y_min, float w, float h, float scale, cocos2d::Size area)
            : _origin(origin), _x_min(x_min), _y_min(y_min), _w(w), _h(h), _scale(scale), _animationProgress01(0) {
        _offset_center_align.x = (area.width - _w * _scale) / 2;
        _offset_center_align.y = (area.height - _h * _scale) / 2;
    }

    void TranformVec2(cocos2d::Vec2 &pos) {
        pos.x = _offset_center_align.x + _origin.x + (pos.x - _x_min) * _scale;
        pos.y = _offset_center_align.y + _origin.y + (pos.y - _y_min) * _scale;
    }


    cocos2d::Vec2 _origin;
    float _x_min, _y_min;
    float _w, _h;
    float _scale;
    cocos2d::Vec2 _offset_center_align;


};


#endif //__TRANSFORMINFO_H__
