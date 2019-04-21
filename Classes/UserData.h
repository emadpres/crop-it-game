#ifndef __USER_DATA_H__
#define __USER_DATA_H__


#include <cocos/base/CCUserDefault.h>

class UserData
{
    static UserData* _instance;

public:
    static UserData* getInstance()
    {
        if (_instance == nullptr)
            _instance = new UserData();

        return _instance;
    }

    void setHighScore(int score) { cocos2d::UserDefault::getInstance()->setIntegerForKey("cropit_high_score", score); }
    int getHighScore() const { return cocos2d::UserDefault::getInstance()->getIntegerForKey("cropit_high_score", 0); }

    void SetCurrentLevel(int currentLevel) { _cuurentLevel = currentLevel; }
    int GetCurrentLevel() const { return _cuurentLevel; }

private:

    int _cuurentLevel;
};


#endif //__USER_DATA_H__
