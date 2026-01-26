#include "Utilities.h"
#include <qDebug>
#include <QtCore/qeventloop.h>
#include <QtCore/qtimer.h>
#include <iomanip>
#include <sstream>
Utilities::Utilities() {}

int Utilities::GenerateRandomHexColor()
{
    static RandomColor randomColor;
    int num = randomColor.generate();
    return (num);
}

std::string Utilities::GenerateRandomHexColorAsString()
{
    std::stringstream stream;
    stream << std::hex << GenerateRandomHexColor();
    std::string convertedToString( stream.str() );
    return convertedToString;

}

void Utilities::QtSleepMilliSeconds(int millisecondsWait)
{
    QEventLoop loop;
    QTimer t;
    t.connect(&t, &QTimer::timeout, &loop, &QEventLoop::quit);
    t.start(millisecondsWait);
    loop.exec();
}

std::string Utilities::FloatToString(float number, int precision)
{
    std::stringstream formattedNumber;
    formattedNumber<<number<<std::setprecision(precision);
    return formattedNumber.str();

}
