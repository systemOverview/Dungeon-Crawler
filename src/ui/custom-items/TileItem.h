#ifndef TILEITEM_H
#define TILEITEM_H

#include <QGraphicsItem>
#include "CharacterItem.h"
#include "GameItem.h"

class TileItem : public GameItem
{
    Q_OBJECT
private:
    // start debugging :
    void drawLines(QPainter* painter);
    //end debugging:
    int m_row = -1;
    int m_column = -1;
    void fixMyPosition() override;
    QTimer* m_doubleClickTimer;

    CharacterItem* m_character = nullptr;
    int m_characterCell = 1;

public:
    TileItem(int row, int col, char textureID);

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    int getRow() const;
    int getColumn() const;
    std::pair<int, int> getCordsAsPair() const;

public slots:
signals:
    void tilePressed(TileItem* tile);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
};

#endif // TILEITEM_H
