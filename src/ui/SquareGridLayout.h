#ifndef SQUAREGRIDLAYOUT_H
#define SQUAREGRIDLAYOUT_H

#include <QObject>
#include "Constants.h"

// This class serves as a layout for graphicsitems inside a view. It takes a view parent, and resize items added through
// addItem() to fill the view as much as possible, without overstepping its boundaries.

class QGraphicsView;
class QGraphicsItem;
class GameItem;
class SquareGridLayout : public QObject
{
private:
    Q_OBJECT
    int m_numberOfRows{};
    int m_numberOfColumns{};
    std::map<Coordinates, GameItem*> m_controlledItems = {};
    QGraphicsView* m_controlledView = nullptr;

    qreal m_lastCellSize{};
    void recalculateCellSize(QRectF availableSpace);

signals:
    void cellSizeChanged(qreal newSize);

public:
    SquareGridLayout(QGraphicsView* view);
    void addGameItem(GameItem* gameItem, Coordinates coordinates);
    bool eventFilter(QObject* watched, QEvent* event) override;
};

#endif // SQUAREGRIDLAYOUT_H
