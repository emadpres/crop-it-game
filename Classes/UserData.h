#ifndef __USER_DATA_H__
#define __USER_DATA_H__


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

    void setHighScore(int score) { _highScore = score; }
    int getHighScore() const { return _highScore; }

private:

    int _highScore;
};


#endif //__USER_DATA_H__
