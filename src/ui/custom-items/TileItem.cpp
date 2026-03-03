#include "TileItem.h"
#include <QMessageBox>
#include <QPainter>
#include <QtCore/qtimer.h>
#include <QtWidgets/qgraphicssceneevent.h>
#include "Constants.h"
#include <SpriteManager.h>

TileItem::TileItem(int row, int col, char textureID)
    : m_coordinates{row, col}
    , GameItem(QPixmap(GUIPaths::TileCharToPathRegister[textureID])) {
    fixMyPosition();
}

void TileItem::fixMyPosition() {
    QGraphicsItem::setPos(m_coordinates.column * SIDE_LENGTH, m_coordinates.row * SIDE_LENGTH);
}

void TileItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        emit tilePressed(this);
        QGraphicsItem::mousePressEvent(event);
        return;
    }

    // QMessageBox debugInfo;
    // QString text;
    // QDebug{&text} << "cords" << m_coordinates;
    // QDebug{&text} << "bounding rect" << boundingRect();
    // QDebug{&text} << "position" << pos();

    // for (auto element : collidingItems()) {
    //     QDebug{&text} << element;
    // }

    // debugInfo.setText(text);
    // // debugInfo.exec();
    QGraphicsItem::mousePressEvent(event);
}

void TileItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    painter->drawPixmap(boundingRect(), m_texture, m_texture.rect());
}

int TileItem::getRow() const { return m_coordinates.row; }

int TileItem::getColumn() const { return m_coordinates.column; }

Coordinates TileItem::getCoordinates() const { return m_coordinates; }

void TileItem::tileTextureChanged(char newTileTexture) {
    m_texture = QPixmap(GUIPaths::TileCharToPathRegister[newTileTexture]);
}

// debugging
void TileItem::drawLines(QPainter* painter) {
    //TEST
    float step = boundingRect().width() / 5;
    float xpos = 0;
    QPen pen;
    pen.setColor(Qt::white);
    pen.setWidth(1);
    painter->setPen(pen);
    for (int i = 0; i < 5; i++) {
        QPoint from(xpos, 10);
        QPoint to(xpos, boundingRect().height());
        painter->drawLine(from, to);
        xpos += step;
    }
}
