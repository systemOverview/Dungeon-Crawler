#include "TileItem.h"
#include <QMessageBox>
#include <QPainter>
#include <QtCore/qtimer.h>
#include <QtWidgets/qgraphicssceneevent.h>
#include "Constants.h"

void TileItem::fixMyPosition() {
    QGraphicsItem::setPos(m_col * SIDE_LENGTH, m_row * SIDE_LENGTH);
}

TileItem::TileItem(int row, int col, char textureID)
    : GameItem(QPixmap(GUIPaths::TileCharToPathRegister[textureID]))
    , m_row(row)
    , m_col(col) {
    fixMyPosition();
}

void TileItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        emit tilePressed(this);
        QGraphicsItem::mousePressEvent(event);
        return;
    }

    QMessageBox debugInfo;
    QString text;

    QDebug{&text} << "bounding rect" << boundingRect();
    QDebug{&text} << "position" << pos();

    for (auto element : collidingItems()) {
        QDebug{&text} << element;
    }

    debugInfo.setText(text);
    debugInfo.exec();
    QGraphicsItem::mousePressEvent(event);
}
