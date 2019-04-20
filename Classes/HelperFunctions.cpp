#include "HelperFunctions.h"

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
