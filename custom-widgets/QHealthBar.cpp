#include "QHealthBar.h"
#include <QStyleOption>
#include <QtGui/qpainter.h>
#include <sstream>

QHealthBar::QHealthBar(QWidget *parent)
    : QWidget{parent}, m_healthPercentage(100)
{
    updateStyleSheet();
    setFixedSize(50, 10);
}

void QHealthBar::paintEvent(QPaintEvent* event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    QWidget::paintEvent(event);
}

void QHealthBar::updateParent(QWidget *parent)
{
    setParent(parent);
    notifyObservers("Health bar parent");
}

void QHealthBar::updateStyleSheet()
{
    float stopRedAt = std::clamp(m_healthPercentage / 100.0f, 0.0f, 1.0f);
    if (stopRedAt < 0) {
        stopRedAt = 0;
        m_healthPercentage = 0;
    }
    float startWhiteFrom = stopRedAt + 0.01;
    if (stopRedAt >= 1) {
        stopRedAt = 0.99;
        startWhiteFrom = 1;
    }
    std::ostringstream style;
    style << "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
          << "stop:0 red, stop:" << stopRedAt << " red, "
          << "stop:" << startWhiteFrom << " white, stop:1 white);"
          << "border:1px solid black;";

    setStyleSheet(QString::fromStdString(style.str()));

}

void QHealthBar::updateHealthBarPercentage(float newPercentage)
{
    m_healthPercentage = newPercentage;
    updateStyleSheet();
}
