#ifndef TILEITEM_H
#define TILEITEM_H

#include <QGraphicsItem>
#include "Constants.h"
#include "GameItem.h"

class TileItem : public GameItem
{
    Q_OBJECT
public:

private:
    Coordinates m_coordinates = {-1, -1};

    Types::TileType m_tileType{};

    void fixMyPosition() override;

public:
    TileItem(Types::TileType tileType, Coordinates coordinates, Mode mode = Mode::Clickable);
    TileItem(Types::TileType tileType, int row, int col, Mode mode = Mode::Clickable);

    Coordinates getCoordinates() const;
    int getRow() const;
    int getColumn() const;

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

public slots:
    void changeTileType(Types::TileType newTileType);
signals:
    void tileClicked(TileItem* tile);

protected:
    void addInfoToDragDropMimeData(DragAndDropGameItemMimeData* mimeData) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void dropEvent(QGraphicsSceneDragDropEvent* event) override;
};

#endif // TILEITEM_H
