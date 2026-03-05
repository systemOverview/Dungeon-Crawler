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

    template<typename enumType>
    static QString Q_ENUM_ToQString(enumType enumValue) {
        QString value = QMetaEnum::fromType<enumType>().valueToKey(int(enumValue));
        return value;
    }

    template<typename enumType>
    static enumType QString_To_Q_ENUM(QString value) {
        auto&& metaEnum = QMetaEnum::fromType<enumType>();
        enumType wantedEnum = static_cast<enumType>(
            metaEnum.keyToValue(value.toStdString().c_str()));
        return wantedEnum;
    }
};

#endif // UTILITIES_H
