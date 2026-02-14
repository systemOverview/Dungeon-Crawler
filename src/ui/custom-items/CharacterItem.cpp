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
#include "SpriteManager.h"
#include "animations/CharacterAnimation.h"
CharacterItem::CharacterItem() {
    setDefaultParts();
    setupStateMachine();
}

void CharacterItem::setDefaultParts() {
    for (int i = 0; i < int(CharacterGraphics::CharacterPart::PAST_END); i++) {
        m_partsGraphicOptions.insert({CharacterGraphics::CharacterPart(i), 0});
    }
}

void CharacterItem::startAnimationLoop() {
    for (int i = 1; i < int(CharacterAnimation::AnimationType::PAST_END);
         i++) { // 0 is idle (no animation)
        CharacterAnimation* animation = new CharacterAnimation(this,
                                                               CharacterAnimation::AnimationType(i));
        m_customizationAnimationLoop->addAnimation(animation);
    }

    //I am unable to get animations to trigger automatically on state change for some reason
    // So for now this loop repeats until the initial state is left (customizingState->playingState).
    connect(m_customizationAnimationLoop, &QSequentialAnimationGroup::finished, [this]() {
        if (m_stateMachine->configuration().contains(m_stateMachine->initialState())) {
            m_customizationAnimationLoop->start();
        }
    });
}

void CharacterItem::setupAnimationMachine() {
    m_animationMachine = new QStateMachine();
    for (int i = 1; i < int(CharacterAnimation::AnimationType::PAST_END); i++) {
        QState* state = new QState(m_animationMachine);
        if (i == 1) m_animationMachine->setInitialState(state);
        CharacterAnimation* animation = new CharacterAnimation(this,
                                                               CharacterAnimation::AnimationType(i));
        connect(state, &QState::entered, animation, [animation]() { animation->start(); });
    }
    m_animationMachine->start();
};

void CharacterItem::resize(qreal newTileLength) { m_size = newTileLength; }

void CharacterItem::assignPart(CharacterGraphics::CharacterPart partType, int whichGraphicsOption) {
    m_partsGraphicOptions.insert_or_assign(partType, whichGraphicsOption);
    update();
}

void CharacterItem::moveTo(QPointF newPos) {}

QRectF CharacterItem::boundingRect() const { return {0, 0, m_size, m_size}; }

void CharacterItem::paint(QPainter* painter,
                          const QStyleOptionGraphicsItem* option,
                          QWidget* widget) {
    for (const auto& [part, partOption] : m_partsGraphicOptions) {
        QPixmap pixmap = SpriteManager::GetFrameFromSprite(part, partOption, m_currentFrameId);
        painter->drawPixmap(boundingRect(), pixmap, pixmap.rect());
    }
}

void CharacterItem::setupStateMachine() {
    m_stateMachine = new QStateMachine();
    QState* customizingState = new QState(m_stateMachine);
    m_customizationAnimationLoop = new QSequentialAnimationGroup;

    QState* playingState = new QState(m_stateMachine);
    QFinalState* deadState = new QFinalState(m_stateMachine);

    connect(customizingState, &QState::entered, m_customizationAnimationLoop, [this]() {
        m_customizationAnimationLoop->start();
    });

    connect(customizingState, &QState::finished, m_customizationAnimationLoop, [this]() {
        m_customizationAnimationLoop->finished();
    });

    m_stateMachine->setInitialState(customizingState);
    m_stateMachine->start();

    startAnimationLoop();
}

void CharacterItem::setCurrentFrameId(int frameId) {
    m_currentFrameId = frameId;
    update();
}
