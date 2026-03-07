#include "TileItem.h"
#include <QDrag>
#include <QMessageBox>
#include <QMimeData>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QtCore/qtimer.h>
#include <QtWidgets/qgraphicseffect.h>
#include <QtWidgets/qgraphicsscene.h>
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

void TileItem::handlePlayerMoveRequest(QGraphicsSceneMouseEvent* event) {
    // During game play , tile is clicked means player wants to move here.

    emit tileClicked(this);
    QGraphicsItem::mousePressEvent(event);
    event->setAccepted(true);
}

TileItem* TileItem::findTileUnderPosition(QPointF scenePosition) const {
    QList<QGraphicsItem*> itemsAtEventPos = scene()->items(scenePosition);

    for (QGraphicsItem* item : std::as_const(itemsAtEventPos)) {
        TileItem* converted = dynamic_cast<TileItem*>(item);
        if (converted != nullptr) {
            return converted; // only one tile can be on a certain pos in game board
        }
    }

    return nullptr; // none found
}

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
    m_tileType = newTileType;
    m_texturePixmap = QPixmap(GUIPaths::TileTypeToPathRegister.at(newTileType));
    update();
}

void TileItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    switch (m_mode) {
    case GameItem::Mode::Clickable: {
        if (event->button() == Qt::LeftButton) {
            handlePlayerMoveRequest(event);
            return;
        }
    }

    case GameItem::Mode::DragAndDropInitiator: {
        if (event->button() == Qt::LeftButton) {
            event->setAccepted(true);
            return;
        }
    }
    case GameItem::Mode::DragAndDropReceiver:
        break;
    }

    if (event->button() == Qt::RightButton && (m_lineOriginatingFromMe == nullptr)) {
        m_lineOriginatingFromMe = new LineConnectingItems(this, nullptr);
        event->setAccepted(true);
    }
}

void TileItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    if (this->getTileType() == Types::TileType::Portal && m_lineOriginatingFromMe != nullptr
        && m_lineOriginatingFromMe->getToItem() == nullptr) {
        m_lineOriginatingFromMe->setToPos(event->scenePos());
    }
    else if (m_mode == GameItem::Mode::DragAndDropInitiator) {
        GameItem::mouseMoveEvent(event);
    }
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

void TileItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    if (m_lineOriginatingFromMe != nullptr) {
        TileItem* tileUnderMouse = findTileUnderPosition(event->scenePos());

        if (tileUnderMouse != nullptr && tileUnderMouse->getTileType() == Types::TileType::Portal) {
            event->accept();

            m_lineOriginatingFromMe->setToItem(tileUnderMouse);
            emit lineCreated(m_lineOriginatingFromMe);
        }

        else {
            delete m_lineOriginatingFromMe;
            m_lineOriginatingFromMe = nullptr;
        }
    }
}
