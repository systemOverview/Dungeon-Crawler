#include "QGameField.h"
#include <QtGui/qpainter.h>
#include <QLayout>
#include <QFontDatabase>
QGameField::QGameField(QWidget* parent)
    : QWidget{parent}
{
}

void QGameField::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QImage image(":/pics/textures/start screen/bloody_frame.png");
    painter.drawImage(rect(), image);
}

void QGameField::resizeEvent(QResizeEvent *event)
{
    if (child){child->setRect(this->layout()->contentsRect());};
    QWidget::resizeEvent(event);
}
