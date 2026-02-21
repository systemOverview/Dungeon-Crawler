#ifndef BUTTONITEM_H
#define BUTTONITEM_H

#include <QtWidgets/qgraphicsitem.h>
class ButtonItem : public QObject, public QGraphicsItem
{
public:
    enum class ButtonType { StartButton, PAST_END };

private:
    Q_OBJECT
    QPixmap m_pixmap;
    qreal m_width = 50;
    qreal m_height = 50;
    ButtonType m_type;

signals:
    void buttonClicked(ButtonItem::ButtonType);

public:
    ButtonItem(QPixmap pixmap, ButtonType type);
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    // QGraphicsItem interface
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
};

#endif // BUTTONITEM_H
