#include "SquareGridLayout.h"

#include <QGraphicsView>
#include <QtWidgets/qgraphicsitem.h>
#include <GameItem.h>
#include <qcoreevent.h>

void SquareGridLayout::recalculateCellSize(QRectF availableSpace) {
    qreal cellSize = std::min(m_controlledView->rect().width() / m_numberOfColumns,
                              m_controlledView->rect().height() / m_numberOfRows);
    if (cellSize != m_lastCellSize) {
        for (auto [coordinates, item] : m_controlledItems) {
            item->setSide(cellSize);
            item->setPos(coordinates.column * cellSize, coordinates.row * cellSize);
        }
        // emit cellSizeChanged(cellSize);
        cellSize = m_lastCellSize;
    }
}

SquareGridLayout::SquareGridLayout(QGraphicsView* view) {
    assert((view != nullptr && view->scene() != nullptr)
           && "Unconfigured view passed to SquareGridLayout.");
    setParent(view);
    m_controlledView = view;
    view->installEventFilter(this);
}

void SquareGridLayout::addGameItem(GameItem* gameItem, Coordinates coordinates) {
    if (coordinates.row + 1 > m_numberOfRows) {
        m_numberOfRows = coordinates.row + 1;
    }

    if (coordinates.column + 1 > m_numberOfColumns) {
        m_numberOfColumns = coordinates.column + 1;
    }

    m_controlledItems.insert_or_assign(coordinates, gameItem);
    m_controlledView->scene()->addItem(gameItem);
    recalculateCellSize(m_controlledView->rect());
    // connect(this, &SquareGridLayout::cellSizeChanged, gameItem, &GameItem::setPos);
}

bool SquareGridLayout::eventFilter(QObject* watched, QEvent* event) {
    if (event->type() == QEvent::Resize) {
        QGraphicsView* view = qobject_cast<QGraphicsView*>(watched);
        if (view == m_controlledView) {
            recalculateCellSize(m_controlledView->rect());
            QGraphicsScene* scene = m_controlledView->scene();
            scene->setSceneRect(scene->itemsBoundingRect());
        }
    }
    return false;
}
