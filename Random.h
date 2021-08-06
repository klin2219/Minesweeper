#pragma once
#include <random>
class Random
{
    static std::mt19937 random;
public:
    // Static means ONE and ONLY one of this function
    // All instances SHARE this function (no duplicates will occur)!
    // They occur for the lifetime of the program, similar but ARE NOT global variables
    static int Int(int min, int max);
    static float Float(float min, float max);

};