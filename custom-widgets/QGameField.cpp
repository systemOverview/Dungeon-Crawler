#include "QGameField.h"
#include <QtGui/qpainter.h>
#include <QLayout>
QGameField::QGameField(QWidget* parent)
    : QWidget{parent}
{
}

void QGameField::paintEvent(QPaintEvent *event)
{

}

void QGameField::resizeEvent(QResizeEvent *event)
{
    if (child){child->setRect(this->layout()->contentsRect());};
    QWidget::resizeEvent(event);
}
