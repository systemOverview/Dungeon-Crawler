#ifndef UTILITIES_H
#define UTILITIES_H
#include <QMetaEnum>
#include "RandomColor.h"
class Utilities
{
private:
public:
    Utilities();
    static int GenerateRandomHexColor(); // returns an int in format 0xRRGGBB.
    static std::string GenerateRandomHexColorAsString();
    static void QtSleepMilliSeconds(int millisecondsWait);
    static int GenerateRandomNumber(int startRange = 0, int endRange = 100);
    static int positiveModulo(int i, int n);

    //debug

    static std::string FloatToString(float number, int precision);
    static void SaveToFile(QPixmap pixmap, QString name = "");
    static void SaveToFile(QImage image, QString name = "");
};

#endif // UTILITIES_H
