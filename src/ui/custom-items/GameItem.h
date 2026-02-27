#ifndef GAMEITEM_H
#define GAMEITEM_H

#include <QGraphicsItem>
#include <QObject>

class GameItem : public QObject, public QGraphicsItem
{
    // All game items have the same dimensions, painting is supposed to be uniform too,
    // MainWindow calculates the new dimension when the view is resized. calls the static function GameItem::setSideLength
    // emits dimensionsChanged (no args) which is connected to item->resize().
    Q_OBJECT

protected:
    inline static qreal SIDE_LENGTH = 70;
    QPixmap m_texture;
    virtual void fixMyPosition() = 0;

public:
    GameItem(QPixmap texture = QPixmap());
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

public slots:
    static void SetSideLength(qreal newSideLength);
    void resize();
};

#endif // GAMEITEM_H
