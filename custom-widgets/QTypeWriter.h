#ifndef QTYPEWRITER_H
#define QTYPEWRITER_H
#include <QTextEdit>
#include "Utilities.h"
class QTypeWriter : public QTextEdit
{
    Q_OBJECT
    inline static int SleepTimeBetweenCharactersInMilliSeconds = 50;
    QString m_text = "";
public:
    QTypeWriter();
    void setText(const QString &text);
    static void setSleepTimeBetweenCharacters(int sleepTimeInMilliSeconds);
};

#endif // QTYPEWRITER_H
