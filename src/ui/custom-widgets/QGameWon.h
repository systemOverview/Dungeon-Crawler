#ifndef QGAMEWON_H
#define QGAMEWON_H

#include <QWidget>
#include <QtWidgets/qlabel.h>

class QGameWon : public QWidget
{
    Q_OBJECT
    QLabel* label = nullptr;
public:
    explicit QGameWon(QWidget* parent = nullptr);

    void paintEvent(QPaintEvent *event);
    void enable();
signals:
};

#endif // QGAMEWON_H
