#include "CharacterItem.h"
#include <QFile>
#include <QFinalState>
#include <QSignalTransition>
#include <QStateMachine>
#include <QtCore/qpropertyanimation.h>
#include <QtCore/qsequentialanimationgroup.h>
#include <QtCore/qtimeline.h>
#include <QtCore/qtimer.h>
#include <QtGui/qbitmap.h>
#include <QtGui/qpainter.h>
#include <QtGui/qpicture.h>
#include "CharacterAnimation.h"
#include "CharacterTile_UI_PlacementMediator.h"
#include "SpriteManager.h"
#include "TileItem.h"
#include <Utilities.h>
CharacterItem::CharacterItem(Character::CharacterType characterType)
    : m_characterType{characterType}
    , GameItem(QPixmap()) {
    m_animation = new CharacterAnimation(this);
    setDefaultParts();
    connect(this, &CharacterItem::stateChanged, m_animation, &CharacterAnimation::updateAnimation);
}

void CharacterItem::setDefaultParts() {
    for (int i = 0; i < int(CharacterItem::CharacterPart::PAST_END); i++) {
        m_partsGraphicOptions.insert({CharacterItem::CharacterPart(i), 0});
    }
}

Character::CharacterType CharacterItem::getCharacterType() const { return m_characterType; }

std::map<CharacterItem::CharacterPart, int> CharacterItem::getPartsGraphicsOptions() const {
    return m_partsGraphicOptions;
}

void CharacterItem::setPartsGraphics(
    std::map<CharacterItem::CharacterPart, int> partsGraphicsOptions) {
    m_partsGraphicOptions = partsGraphicsOptions;
}

QPixmap CharacterItem::getPixmap() const {
    QPixmap base = QPixmap();

    for (const auto& [part, partOption] : m_partsGraphicOptions) {
        QPixmap pixmap = SpriteManager::GetFrameFromSprite(m_characterType,
                                                           part,
                                                           partOption,
                                                           m_currentFrameId);
        if (pixmap.isNull()) {
            continue;
        }
        if (part == CharacterPart::Base) {
            base = pixmap;
        }
        else {
            QPainter painter = QPainter(&base);
            painter.drawPixmap(base.rect(), pixmap);
        }
    }
    return base;
}

void CharacterItem::fixMyPosition() {
    if (scene()) {
        QPointF pos = CharacterTile_UI_PlacementMediator::GetCharacterPosition(this);
        setPos(pos);
    }
}

void CharacterItem::assignPart(CharacterItem::CharacterPart partType, int whichGraphicsOption) {
    m_partsGraphicOptions.insert_or_assign(partType, whichGraphicsOption);
    update();
}

void CharacterItem::paint(QPainter* painter,
                          const QStyleOptionGraphicsItem* option,
                          QWidget* widget) {

    QPixmap pixmap = getPixmap();
    QImage img = pixmap.toImage();
    SpriteManager::TrimTransparent(img);
    pixmap = QPixmap::fromImage(img);

    float characterHeight = SIDE_LENGTH * 80 / 100;

    painter->drawPixmap(QRectF{0,
                               SIDE_LENGTH - characterHeight,
                               boundingRect().width(),
                               characterHeight},
                        pixmap,
                        pixmap.rect());

    QPixmap healthBarBackground(GUIPaths::HealthBarBackground);
    QPixmap healthBarInner(GUIPaths::HealthBarInner);

    QPainter healthPainter(&healthBarBackground);
    healthPainter.drawPixmap(healthBarBackground.rect(), healthBarInner);

    painter->drawPixmap(QRectF(0, 0, boundingRect().width(), SIDE_LENGTH - characterHeight),
                        healthBarBackground,
                        healthBarBackground.rect());
}

int CharacterItem::getCurrentFrameID() const { return m_currentFrameId; }

void CharacterItem::setCurrentFrameID(int frameId) {
    m_currentFrameId = frameId;
    update();
}

void CharacterItem::setState(State newState, QPointF newPosition) {
    m_state = newState;
}

void CharacterItem::AnimateMove(Coordinates fromTileCoords, Coordinates ToTileCoords) {
    m_animation->animateMove(fromTileCoords, ToTileCoords);
}

void CharacterItem::setTile(TileItem* tile) { m_tile = tile; }

TileItem* CharacterItem::getTile() const { return m_tile; }

QPointF CharacterItem::getNewPosition() const {
    return m_newPosition;
}

void CharacterItem::setNewPosition(QPointF newNewPosition) { m_newPosition = newNewPosition; }

CharacterItem::State CharacterItem::getState() const { return m_state; }

void CharacterItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    QGraphicsItem::mousePressEvent(event);
}

