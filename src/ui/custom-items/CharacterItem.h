#ifndef CHARACTERITEM_H
#define CHARACTERITEM_H

#include <QGraphicsItem>
#include <QObject>
#include "Constants.h"

class CharacterItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
    inline static int SECONDS_PER_MOVE = 2;
    QTimer* m_timer;
    QString m_texutreBase = ":/graphics/characters/sprite/0.png";
    int m_counter = 1;
    int m_lastStep = 1;
    int m_startingPos = 0;
    qreal m_size = 300;
    int m_selectedArmor = 1;
    std::map<CharacterWearables::WearableType, QPixmap> m_wearables = {};

public slots:
    void resize(qreal newTileLength);
    void animateMove(QPointF newPos);
    void assignWearable(CharacterWearables::WearableType wearableType, int wearableID);

public:
    enum { Type = UserType + 2 };

    int type() const override { return Type; }
    CharacterItem();
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
};

#endif // CHARACTERITEM_H
