#include "GameItem.h"
#include <QtGui/qpainter.h>

void GameItem::SetSideLength(qreal newSideLength) { SIDE_LENGTH = newSideLength; }

GameItem::GameItem(QPixmap texture, qreal customSideLength)
    : m_texture{texture}
    , m_customSideLength{customSideLength} {}

QRectF GameItem::boundingRect() const {
    if (m_customSideLength > 0) {
        return {0, 0, m_customSideLength, m_customSideLength};
    }
    return {0, 0, SIDE_LENGTH, SIDE_LENGTH};
}

void GameItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    painter->drawPixmap(this->boundingRect(), m_texture, m_texture.rect());
}

int GameItem::getCustomSideLength() { return m_customSideLength; }

void GameItem::setCustomSideLength(qreal newLength) { m_customSideLength = newLength; }

void GameItem::resize() {
    fixMyPosition();
    update();
}
