#include "GameItem.h"
#include <QCursor>
#include <QGraphicsSceneMouseEvent>
#include <QtGui/qpainter.h>
#include "QGraphicsView"
#include <qapplication.h>
#include <qdrag.h>

QPixmap GameItem::getPixmap() const { return m_texturePixmap; }

bool GameItem::isMoveEventTooShortToMatter(QGraphicsSceneMouseEvent* event) {
    if (QLineF(event->screenPos(), event->buttonDownScreenPos(Qt::LeftButton)).length()
        < QApplication::startDragDistance()) {
        return true;
    }
    return false;
}

DragAndDropGameItemMimeData* GameItem::createMimeData() {
    DragAndDropGameItemEvent event;
    event.setDraggedItem(this);

    DragAndDropGameItemMimeData* mimeData = new DragAndDropGameItemMimeData();
    mimeData->event = event;

    addInfoToDragDropMimeData(mimeData); //allow subclasses to change it

    return mimeData;
}

GameItem::GameItem(Mode mode, QPixmap texturePixmap)
    : m_mode{mode}
    , m_texturePixmap{texturePixmap} {
    if (m_mode == Mode::DragAndDropInitiator) {
        setToolTip("Drag and drop");
        setCursor(Qt::OpenHandCursor);
    }
    else if (m_mode == Mode::DragAndDropReceiver) {
        setAcceptDrops(true);
    }
}

GameItem::GameItem(Mode mode, QString texturePath)
    : GameItem(mode, QPixmap(texturePath)) {}
void GameItem::setSideLength(qreal newSideLength) {
    m_sideLength = newSideLength;
    resize();
}

QRectF GameItem::boundingRect() const { return {0, 0, m_sideLength, m_sideLength}; }

void GameItem::setSide(qreal sideLength) { m_sideLength = sideLength; }

void GameItem::setDragAndDropPixmapSize(qreal size) {
    m_dragAndDropPixmapSize = size;
    m_isDragAndDropPixmapSizeSet = true;
}

void GameItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    painter->drawPixmap(this->boundingRect(), m_texturePixmap, m_texturePixmap.rect());
}

void GameItem::resize() {
    fixMyPosition();
    update();
}

void GameItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    if (isMoveEventTooShortToMatter(event)) {
        return;
    }

    QDrag* drag = new QDrag(event->widget());
    DragAndDropGameItemMimeData* mimeData = createMimeData();
    drag->setMimeData(mimeData);

    if (m_isDragAndDropPixmapSizeSet) {
        drag->setPixmap(this->getPixmap().scaled(m_dragAndDropPixmapSize, m_dragAndDropPixmapSize));
    }
    else {
        drag->setPixmap(this->getPixmap().scaled(m_sideLength, m_sideLength));
    }
    drag->setHotSpot(drag->pixmap().rect().center());
    drag->exec();
}
void GameItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    if (event->button() == Qt::LeftButton && m_mode == Mode::DragAndDropInitiator) {
        event->setAccepted(true);
    }
}

void GameItem::dragEnterEvent(QGraphicsSceneDragDropEvent* event) {
    event->setAccepted(true);
    update();
}
