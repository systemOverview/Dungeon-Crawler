#ifndef QHEALTHBAR_H
#define QHEALTHBAR_H

#include <QWidget>
#include "Subject.h"

class QHealthBar : public QWidget, public Subject
{
    Q_OBJECT
private:
    float m_healthPercentage;
public:
    QHealthBar(QWidget* parent, float healthPercentage=100);
    void paintEvent(QPaintEvent* event); // QWidget subclasses must have this function explicity included to use stylesheets.
    void updateParent(QWidget* parent);
    void updateStyleSheet();
    void updateHealthBarPercentage(float newPercentage);
signals:
};

#endif // QHEALTHBAR_H
