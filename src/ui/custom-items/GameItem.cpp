#include "GameItem.h"
#include <QtGui/qpainter.h>

void GameItem::SetSideLength(qreal newSideLength) { SIDE_LENGTH = newSideLength; }

GameItem::GameItem(QPixmap texture)
    : m_texture{texture} {}

QRectF GameItem::boundingRect() const { return {0, 0, SIDE_LENGTH, SIDE_LENGTH}; }

void GameItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    painter->drawPixmap(this->boundingRect(), m_texture, m_texture.rect());
}

void GameItem::resize() {
    fixMyPosition();
    update();
}
