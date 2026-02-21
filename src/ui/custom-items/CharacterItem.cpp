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
#include "SpriteManager.h"
#include "TileItem.h"

CharacterItem::CharacterItem(CharacterItem::CharacterType characterType)
    : m_characterType{characterType}
    , GameItem(QPixmap(), -1) {
    setDefaultParts();
    m_animation = new CharacterAnimation(this);
    connect(this, &CharacterItem::stateChanged, m_animation, &CharacterAnimation::updateAnimation);
}

void CharacterItem::setDefaultParts() {
    for (int i = 0; i < int(CharacterItem::CharacterPart::PAST_END); i++) {
        m_partsGraphicOptions.insert({CharacterItem::CharacterPart(i), 0});
    }
}

void CharacterItem::fixMyPosition() {}

void CharacterItem::assignPart(CharacterItem::CharacterPart partType, int whichGraphicsOption) {
    m_partsGraphicOptions.insert_or_assign(partType, whichGraphicsOption);
    update();
}

void CharacterItem::paint(QPainter* painter,
                          const QStyleOptionGraphicsItem* option,
                          QWidget* widget) {
    for (const auto& [part, partOption] : m_partsGraphicOptions) {
        QPixmap pixmap = SpriteManager::GetFrameFromSprite(m_characterType,
                                                           part,
                                                           partOption,
                                                           m_currentFrameId);
        painter->drawPixmap(boundingRect(), pixmap, pixmap.rect());
    }
}

int CharacterItem::getCurrentFrameID() const { return m_currentFrameId; }

void CharacterItem::setCurrentFrameID(int frameId) {
    m_currentFrameId = frameId;
    update();
}

void CharacterItem::setState(State newState, QPointF newPosition) {
    if (newState != State::Looping) {
        setCustomSideLength(-1);    // stop having a custom side length
        setZValue(1);
        m_newPosition = pos() + newPosition;
    }

    m_state = newState;
    emit stateChanged(newState);
}

void CharacterItem::setTile(TileItem* tile) { m_tile = tile; }

TileItem* CharacterItem::getTile() const { return m_tile; }

QPointF CharacterItem::getNewPosition() const {
    return m_newPosition;
}

void CharacterItem::setNewPosition(QPointF newNewPosition) { m_newPosition = newNewPosition; }

CharacterItem::State CharacterItem::getState() const { return m_state; }
