#ifndef QGAMEFIELD_H
#define QGAMEFIELD_H

#include <QWidget>
#include <QtGui/qpainter.h>
#include "QOverLay.h"
class QGameField : public QWidget
{
    Q_OBJECT
    QWidget* start;
    QWidget* end;
    QOverlay* child;
public:
    void setChild(QOverlay* c){child=c;};
    explicit QGameField(QWidget* parent = nullptr);
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent *event) override;
};

#endif // QGAMEFIELD_H
