#ifndef UTILITIES_H
#define UTILITIES_H
#include "RandomColor.h"
class Utilities
{
public:
    Utilities();
    static int GenerateRandomHexColor(); // returns an int in format 0xRRGGBB.
    static std::string GenerateRandomHexColorAsString();
    static void QtSleepMilliSeconds(int millisecondsWait);
    static std::string FloatToString(float number, int precision);


};

#endif // UTILITIES_H
