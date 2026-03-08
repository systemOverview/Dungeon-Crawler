#ifndef CHARACTERITEM_H
#define CHARACTERITEM_H

#include <QGraphicsItem>
#include <QObject>
#include <QtCore/qobject.h>
#include <QtCore/qsequentialanimationgroup.h>
#include <QtStateMachine/qstatemachine.h>
#include "GameItem.h"
#include "Types.h"
class CharacterAnimation;
class TileItem;
class SpriteManager;
struct Coordinates;
class CharacterItem : public GameItem

{
    Q_OBJECT

    friend TileItem;

public:
    enum class CharacterPart { Base, Head, Outfit, Weapon, PAST_ENUM_END };
    enum class PositionUpdateReason { WindowResize, CharacterMovement };

    Q_ENUM(CharacterPart);
    Q_ENUM(PositionUpdateReason);

private:
    struct CharacterOrientation
    {
        int horizontalFlip = 1;
        qreal verticalRotation = 0;

        void reset(bool resetHorizontally = true, bool resetVertically = true) {
            if (resetHorizontally == true) horizontalFlip = 1;
            if (resetVertically == true) verticalRotation = 0;
        }
    };

    std::vector<CharacterAnimation*> m_animationsQueue = {};
    void playNextAnimation();
    CharacterOrientation m_characterOrientation;
    void updateOrientation(QPointF oldPos, QPointF newPos);

    inline static constexpr int STANDING_FRAME = 10;
    int m_currentFrameId{STANDING_FRAME};

    Types::CharacterType m_characterType;
    int m_characterID{};

    float m_healthPercentage = 100;
    bool m_isHealthbarShown = false;

    std::map<CharacterItem::CharacterPart, int> m_partsGraphicOptions = {};

    TileItem* m_tile = nullptr;

    void setDefaultParts();
    QPixmap getPixmap() const override;
    CharacterAnimation* m_currentAnimation = nullptr;
    PositionUpdateReason m_lastPositionUpdateReason{
              CharacterItem::PositionUpdateReason::WindowResize};

public slots:
    void assignPart(CharacterItem::CharacterPart partType, int whichGraphicsOption);
    void addAnimationToQueue(CharacterAnimation* animation);

public:
    CharacterItem(Types::CharacterType characterType,
                  int characterID,
                  bool isHealthbarShown = false,
                  Mode mode = GameItem::Mode::Clickable);
    void lookTowardsPosition(QPointF pos);
    Types::CharacterType getCharacterType() const;
    int getCharacterID() const;

    TileItem* getTile() const;
    void setTile(TileItem* tile);

    void fixMyPosition() override;
    void updatePosition(
        QPointF newPosition,
        PositionUpdateReason reason = CharacterItem::PositionUpdateReason::WindowResize);

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    std::map<CharacterItem::CharacterPart, int> getPartsGraphicsOptions() const;
    void setPartsGraphics(std::map<CharacterItem::CharacterPart, int> partsGraphicsOptions);

    int getCurrentFrameID() const;
    void setCurrentFrameID(int frameId);
    void setHealthPercentage(float newHealthPercentage);

    ~CharacterItem();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
};

#endif // CHARACTERITEM_H
