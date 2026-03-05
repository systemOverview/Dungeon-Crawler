#ifndef GAMEITEM_H
#define GAMEITEM_H

#include <QGraphicsItem>
#include <QObject>
#include <qmimedata.h>

class GameItem;
class DragAndDropGameItemEvent
{
private:
    GameItem* m_draggedItem = nullptr;
    GameItem* m_droppedOnItem = nullptr;

public:
    DragAndDropGameItemEvent(GameItem* draggedItem = nullptr, GameItem* droppedOnItem = nullptr)
        : m_draggedItem(draggedItem)
        , m_droppedOnItem(droppedOnItem) {}

    GameItem* getDraggedItem() const { return m_draggedItem; }
    void setDraggedItem(GameItem* newDraggedItem) { m_draggedItem = newDraggedItem; }

    GameItem* getDroppedOnItem() const { return m_droppedOnItem; }
    void setDroppedOnItem(GameItem* newDroppedOnItem) { m_droppedOnItem = newDroppedOnItem; }
};
class DragAndDropGameItemMimeData : public QMimeData
{
public:
    DragAndDropGameItemEvent event;
};

class GameItem : public QObject, public QGraphicsItem
{
    // All game items have the same dimensions, painting is supposed to be uniform too,
    // MainWindow calculates the new dimension when the view is resized. calls the static function GameItem::setSideLength
    // emits dimensionsChanged (no args) which is connected to item->resize().
    Q_OBJECT
public:
    enum class Mode { Clickable, DragAndDropInitiator, DragAndDropReceiver };

protected:
    Mode m_mode{};
    qreal m_sideLength = 70;
    QPixmap m_texturePixmap;
    bool isMoveEventTooShortToMatter(QGraphicsSceneMouseEvent* event);
    virtual void fixMyPosition() = 0;

public:
    GameItem(Mode mode, QPixmap texturePixmap = QPixmap());
    GameItem(Mode mode, QString texturePath);

    QRectF boundingRect() const override;
    void setSide(qreal sideLength);
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

public slots:
    void setSideLength(qreal newSideLength);
    void resize();
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void dragEnterEvent(QGraphicsSceneDragDropEvent* event) override;
signals:
    void dragAndDropGameItemEvent(DragAndDropGameItemEvent event);
};

class GameItemMimeData : public QMimeData
{
    GameItem* m_receiveingGameItem = nullptr;

public:
    GameItem* getReceivingGameItem() const { return m_receiveingGameItem; }
    void setReceivingGameItem(GameItem* receiveingGameItem) {
        m_receiveingGameItem = receiveingGameItem;
    }
};
#endif // GAMEITEM_H
