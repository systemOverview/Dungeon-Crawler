#ifndef TILEITEM_H
#define TILEITEM_H

#include <QGraphicsItem>
#include "GameItem.h"

class TileItem : public GameItem
{
    Q_OBJECT
private:
    inline static int counter = 0;
    QPixmap m_texture;
    int m_row;
    int m_col;
    void fixMyPosition() override;
    QTimer* m_doubleClickTimer;

public:
    TileItem(int row, int col, char textureID);

public slots:
signals:
    void tilePressed(TileItem* tile);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
};

#endif // TILEITEM_H
