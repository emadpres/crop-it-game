#include "GameOptions.h"

GameOptions& GameOptions::getInstance()
{
    static GameOptions instance;
    return instance;
}