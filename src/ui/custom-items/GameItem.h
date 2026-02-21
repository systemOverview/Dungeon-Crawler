#ifndef GAMEITEM_H
#define GAMEITEM_H

#include <QGraphicsItem>
#include <QObject>

class GameItem : public QObject, public QGraphicsItem
{
    // All game items have the same dimensions, painting is supposed to be uniform too,
    // MainWindow calculates the new dimension when the view is resized. calls the static function GameItem::setSideLength
    // emits dimensionsChanged (no args) which is connected to item->resize().
    //
    // fixMyPosition() is pure since its determined by the type of the item.
    Q_OBJECT
    Q_PROPERTY(qreal customSideLength READ getCustomSideLength WRITE setCustomSideLength)

protected:
    inline static qreal SIDE_LENGTH = 70;
    qreal m_customSideLength
        = -1; // if a texture has a custom length > 0, that length is used instead of the global one.
    QPixmap m_texture;
    virtual void fixMyPosition() = 0;

public:
    GameItem(QPixmap texture = QPixmap(), qreal customSideLength = -1);
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    int getCustomSideLength();
    void setCustomSideLength(qreal newLength);

public slots:
    static void SetSideLength(qreal newSideLength);
    void resize();
};

#endif // GAMEITEM_H
