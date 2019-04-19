#ifndef __GAME_OPTIONS_H__
#define __GAME_OPTIONS_H__

#include <string>
#include <vector>

class GameOptions
{
    static GameOptions* _instance;

public:
    static GameOptions* getInstance()
    {
        if (_instance == nullptr)
            _instance = new GameOptions();

        return _instance;
    }

    void setSoundStatus(bool status) { _soundStatus = status; }
    bool getSoundStatus() const { return _soundStatus; }

    void setMainFont(std::string mainFont) { _mainFont = mainFont; }
    std::string getMainFont() const { return _mainFont; }

    void pushToUrlVector(std::string url) { _urls.push_back(url); }
    std::vector<std::string> getUrls() const { return _urls; }

private:
    GameOptions() {};

    GameOptions(const GameOptions&) = delete;
    void operator=(const GameOptions&) = delete;

    bool _soundStatus;
    std::string _mainFont;
    std::vector<std::string> _urls; // gitPageGame, emad, mojtaba, vahid
};

#endif