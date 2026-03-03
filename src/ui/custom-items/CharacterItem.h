#ifndef CHARACTERITEM_H
#define CHARACTERITEM_H

#include <QGraphicsItem>
#include <QObject>
#include <QtCore/qobject.h>
#include <QtCore/qsequentialanimationgroup.h>
#include <QtStateMachine/qstatemachine.h>
#include "Character.h"
#include "GameItem.h"
class CharacterAnimation;
class TileItem;
class SpriteManager;
struct Coordinates;
class CharacterItem : public GameItem

{
    Q_OBJECT

    friend TileItem;

public:
    enum class CharacterPart { Base, Head, Outfit, Weapon, PAST_END };
    Q_ENUM(CharacterPart);

private:
    std::vector<CharacterAnimation*> m_animationsQueue = {};
    void playNextAnimation();
    struct CharacterOrientation
    {
        int horizontalFlip = 1;
        qreal verticalRotation = 0;

        void reset() {
            horizontalFlip = 1;
            verticalRotation = 0;
        }
    };
    CharacterOrientation m_characterOrientation;
    void updateOrientation(QPointF oldPos, QPointF newPos);

    inline static constexpr int STANDING_FRAME = 10;
    int m_currentFrameId{STANDING_FRAME};

    Character::CharacterType m_characterType;
    int m_characterID{};

    float m_healthPercentage = 100;

    std::map<CharacterItem::CharacterPart, int> m_partsGraphicOptions = {};

    TileItem* m_tile = nullptr;

    void setDefaultParts();
    QPixmap getPixmap() const;
    CharacterAnimation* m_currentAnimation = nullptr;

public slots:
    void assignPart(CharacterItem::CharacterPart partType, int whichGraphicsOption);
    void addAnimationToQueue(CharacterAnimation* animation);

signals:

public:
    CharacterItem(Character::CharacterType characterType, int characterID);
    Character::CharacterType getCharacterType() const;

    TileItem* getTile() const;
    void setTile(TileItem* tile);

    void fixMyPosition() override;

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    std::map<CharacterItem::CharacterPart, int> getPartsGraphicsOptions() const;
    void setPartsGraphics(std::map<CharacterItem::CharacterPart, int> partsGraphicsOptions);

    int getCurrentFrameID() const;
    void setCurrentFrameID(int frameId);
    void setHealthPercentage(float newHealthPercentage);

    ~CharacterItem();

//DEBUGGING INTERFACE TEST
protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;

public:
    bool d_flip = true;
    QMenu* d_contextMenu;
    void d_createMenu();
};

#endif // CHARACTERITEM_H
