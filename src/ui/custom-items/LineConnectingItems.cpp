#include "LineConnectingItems.h"
#include <QtGui/qpainter.h>
#include <QtWidgets/qgraphicsscene.h>


void LineConnectingItems::setToItem(QGraphicsItem* newToItem) {
    m_toItem = newToItem;
    updatePosition();
}


void LineConnectingItems::setToPos(QPointF pos) {
    QPointF from = mapFromItem(m_fromItem, m_fromItem->boundingRect().center());
    QPointF to = mapFromScene(pos);

    setLine({from, to});
    update();
}

QGraphicsItem* LineConnectingItems::getFromItem() const { return m_fromItem; }

QGraphicsItem* LineConnectingItems::getToItem() const { return m_toItem; }

void LineConnectingItems::updatePosition() {
    QPointF from = mapFromItem(m_fromItem, m_fromItem->boundingRect().center());
    QPointF to = mapFromItem(m_toItem, m_fromItem->boundingRect().center());
    setLine({from, to});
}

LineConnectingItems::LineConnectingItems(QGraphicsItem* fromItem, QGraphicsItem* toItem)
    : m_fromItem(fromItem)
    , m_toItem(toItem) {
    m_fromItem->scene()->addItem(this);

    connect(scene(), &QGraphicsScene::sceneRectChanged, this, &LineConnectingItems::updatePosition);
}
