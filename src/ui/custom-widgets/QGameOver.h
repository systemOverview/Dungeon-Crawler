#ifndef QGAMEOVER_H
#define QGAMEOVER_H

#include <QWidget>
#include <QtWidgets/qlabel.h>

class QGameOver : public QWidget
{
    Q_OBJECT
    QLabel* label;
public:
    explicit QGameOver(QWidget* parent = nullptr);
    void paintEvent(QPaintEvent* event) override;
    void enable();

signals:
};

#endif // QGAMEOVER_H
