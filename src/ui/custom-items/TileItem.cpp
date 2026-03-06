#include "TileItem.h"
#include <QDrag>
#include <QMessageBox>
#include <QMimeData>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QtCore/qtimer.h>
#include <QtWidgets/qgraphicseffect.h>
#include <QtWidgets/qgraphicssceneevent.h>
#include "Constants.h"
#include "Utilities.h"
#include <SpriteManager.h>
#include <qapplication.h>

TileItem::TileItem(Types::TileType tileType, Coordinates coordinates, Mode mode)
    : m_tileType{tileType}
    , m_coordinates{coordinates}
    , GameItem(mode, GUIPaths::TileTypeToPathRegister.at(tileType)) {
    setAcceptHoverEvents(true);
    fixMyPosition();
}

TileItem::TileItem(Types::TileType tileType, int row, int col, Mode mode)
    : TileItem(tileType, {row, col}, mode) {}

Types::TileType TileItem::getTileType() const { return m_tileType; }

void TileItem::fixMyPosition() {
    QGraphicsItem::setPos(m_coordinates.column * m_sideLength, m_coordinates.row * m_sideLength);
}

Coordinates TileItem::getCoordinates() const { return m_coordinates; }

int TileItem::getRow() const { return m_coordinates.row; }

int TileItem::getColumn() const { return m_coordinates.column; }

void TileItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    painter->drawPixmap(boundingRect(), m_texturePixmap, m_texturePixmap.rect());
}

void TileItem::changeTileType(Types::TileType newTileType) {
    m_texturePixmap = QPixmap(GUIPaths::TileTypeToPathRegister.at(newTileType));
    update();
}

void TileItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        if (m_mode == Mode::DragAndDropInitiator) {
            event->setAccepted(true);
            return;
        }
        emit tileClicked(this);
        QGraphicsItem::mousePressEvent(event);
        event->setAccepted(true);
    }
    QGraphicsItem::mousePressEvent(event);
}

void TileItem::dropEvent(QGraphicsSceneDragDropEvent* event) {
    const DragAndDropGameItemMimeData* customMimeData
        = dynamic_cast<const DragAndDropGameItemMimeData*>(event->mimeData());
    if (customMimeData != nullptr) {
        DragAndDropGameItemEvent copy = customMimeData->event;
        copy.setDroppedOnItem(this);
        emit dragAndDropGameItemEvent(copy);
    }


    event->accept();
}

void TileItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) {}

void TileItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event) {}
