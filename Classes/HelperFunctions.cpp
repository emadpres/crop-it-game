#include "HelperFunctions.h"
#include "cocos2d.h"
USING_NS_CC;

float GetRand01() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> dis(0, 1);
    return dis(gen);
}

int GetRand1N(int n)
{
    std::random_device r;
    std::default_random_engine el(r());
    std::uniform_int_distribution<int> uniform_dist(1, n);
    return uniform_dist(el);
}


void TranslateDir(int dir, Vec2 &dir1, Vec2 &dir2)
{
    switch (dir) {
        case 1: // Right,Down
            dir1 = Vec2(1, 0);
            dir2 = Vec2(0, -1);
            break;
        case 2:
            dir1 = Vec2(0, -1);
            dir2 = Vec2(-1, 0);
            break;
        case 3:
            dir1 = Vec2(-1, 0);
            dir2 = Vec2(0, 1);
            break;
        case 4:
            dir1 = Vec2(0, 1);
            dir2 = Vec2(1, 0);
            break;
        case 5:
            dir1 = Vec2(0, 1);
            dir2 = Vec2(0, -1);
            break;
        case 6:
        default:
            dir1 = Vec2(1, 0);
            dir2 = Vec2(-1, 0);
            break;
    }
}