#ifndef CHARACTERITEM_H
#define CHARACTERITEM_H

#include <QGraphicsItem>
#include <QObject>
#include <QtCore/qobject.h>
#include <QtCore/qsequentialanimationgroup.h>
#include <QtStateMachine/qstatemachine.h>
#include "ButtonItem.h"
#include "GameItem.h"

class CharacterAnimation;
class TileItem;
class SpriteManager;
class CharacterItem : public GameItem
{
    Q_OBJECT
    Q_PROPERTY(QPointF newPosition READ getNewPosition WRITE setNewPosition)

public:
    enum class State { Idle, Walk, Jump, Punch, Looping, PAST_END };
    enum class CharacterType { Human, Goblin };
    enum class CharacterPart { Base, Head, Outfit, Weapon, PAST_END };

    Q_ENUM(CharacterPart);

private:
    // The character only stores the current frame id (the same for every part)
    // and the user-chosen graphics option for each part. When painting, it requests the images for each part from the
    // sprite manager, which has a cache system in place to avoid re-generating images.

    CharacterItem::CharacterType m_characterType;

    int m_currentFrameId = 0;

    TileItem* m_tile = nullptr;
    std::map<CharacterItem::CharacterPart, int> m_partsGraphicOptions = {};

    QSequentialAnimationGroup* m_customizationAnimationLoop = nullptr;
    CharacterAnimation* m_animation = nullptr;

    QStateMachine* m_stateAnimationMachine = nullptr;
    QStateMachine* m_movingAnimationMachine = nullptr;

    State m_state = CharacterItem::State::Looping;
    bool m_animationLoopingStatus = false;
    void setDefaultParts();
    void setupMovingStateMachine();

    QPointF m_newPosition;

public slots:
    void assignPart(CharacterItem::CharacterPart partType, int whichGraphicsOption);
    void setState(CharacterItem::State, QPointF newPosition = {0, 0});

signals:
    void move();
    void startMovingAnimation();
    void stateChanged(CharacterItem::State newState);

public:
    CharacterItem(CharacterItem::CharacterType characterType);
    void setTile(TileItem* tile);
    TileItem* getTile() const;
    void fixMyPosition() override;
    enum { Type = UserType + 2 };
    int type() const override { return Type; }
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    int getCurrentFrameID() const;
    void setCurrentFrameID(int frameId);
    void startAnimationLoop();
    QPointF getNewPosition() const;
    void setNewPosition(QPointF newNewPosition);
    State getState() const;
    QVariant myColorInterpolator(const std::vector<int>& start,
                                 const std::vector<int>& end,
                                 qreal progress);
};

#endif // CHARACTERITEM_H
