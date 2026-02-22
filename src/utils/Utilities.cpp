#include "Utilities.h"
#include <QtCore/qeventloop.h>
#include <QtCore/qtimer.h>
#include <QtGui/qimage.h>
#include <QtGui/qpixmap.h>
#include <iomanip>
#include <qDebug>
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

int Utilities::GenerateRandomNumber(int startRange, int endRange) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(startRange, endRange);

    return dist(rng);
}

std::string Utilities::FloatToString(float number, int precision)
{
    std::stringstream formattedNumber;
    formattedNumber<<number<<std::setprecision(precision);
    return formattedNumber.str();

}

void Utilities::SaveToFile(QPixmap pixmap, QString name) {
    QString path = "/Users/mbk/prak/src/ui/output/";
    if (name.isEmpty()) {
        path += QString::number(GenerateRandomNumber());
    }
    else {
        path += name;
    }
    path += ".png";
    pixmap.save(path);
}

void Utilities::SaveToFile(QImage image, QString name) {}
