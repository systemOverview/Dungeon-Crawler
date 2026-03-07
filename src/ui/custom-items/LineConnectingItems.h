#ifndef LINECONNECTINGITEMS_H
#define LINECONNECTINGITEMS_H

#include <QGraphicsItem>
#include <QObject>

class LineConnectingItems : public QObject, public QGraphicsLineItem
{
    Q_OBJECT
private:
    QGraphicsItem* m_fromItem = nullptr;
    QGraphicsItem* m_toItem = nullptr;
public slots:
    void updatePosition();

public:
    LineConnectingItems(QGraphicsItem* fromItem, QGraphicsItem* toItem = nullptr);

    void setToItem(QGraphicsItem* newToItem);
    void setToPos(QPointF pos);

    QGraphicsItem* getFromItem() const;
    QGraphicsItem* getToItem() const;
};

#endif // LINECONNECTINGITEMS_H
