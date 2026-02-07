#ifndef QTYPEWRITER_H
#define QTYPEWRITER_H
#include <QTextEdit>
#include "Utilities.h"
#include "Constants.h"
class QTypeWriter : public QTextEdit
{
    Q_OBJECT
    QString m_text = "";
public:
    QTypeWriter();
    void setText(const QString &text);
};

#endif // QTYPEWRITER_H
