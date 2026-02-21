#include "ButtonItem.h"
#include <QPainter>
ButtonItem::ButtonItem(QPixmap pixmap, ButtonType type)
    : m_pixmap{pixmap}
    , m_type{type} {}

QRectF ButtonItem::boundingRect() const { return {0, 0, m_width, m_height}; }

void ButtonItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    painter->drawPixmap(boundingRect(), m_pixmap, m_pixmap.rect());
}

void ButtonItem::mousePressEvent(QGraphicsSceneMouseEvent* event) { emit buttonClicked(m_type); }
