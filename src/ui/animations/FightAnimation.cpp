#include "FightAnimation.h"
#include "CharacterItem.h"

#include <QStateMachine>
#include <QtCore/qtimeline.h>

void FightAnimation::start() {
    float framesCount = m_animationFrames.size();

    m_timeline->setFrameRange(0, framesCount - 1);
    m_timeline->setDuration((framesCount / GameSettings::FRAMES_PER_SECOND) * 1000);
    m_timeline->setCurrentTime(
        m_timeline
            ->duration()); // Workaround for a known unfixed bug https://qt-project.atlassian.net/browse/QTBUG-41610
    connect(m_timeline, &QTimeLine::frameChanged, this, &FightAnimation::playFrame);
    m_timeline->start();
}


FightAnimation::FightAnimation(CharacterItem* character, AnimationType animationType)
    : CharacterAnimation{character, animationType} {}


//attacking animation.
void FightAttackingAnimation::playFrame(int iterator) {
    m_character->setCurrentFrameID(m_animationFrames[iterator]);
    if (iterator == WHICH_STEP_TO_PUNCH) {
        emit iPunchedDefender();
    }
    if (iterator == m_timeline->endFrame()) {
        emit finished();
    }
}

FightAttackingAnimation::FightAttackingAnimation(CharacterItem* character)
    : FightAnimation(character, AnimationType::Attacking) {}

//defending animation.
void FightDefendingAnimation::playFrame(int iterator) {
    m_character->setCurrentFrameID(m_animationFrames[iterator]);
    if (iterator == WHICH_STEP_TO_WAIT_FOR_PUNCH) {
        m_timeline->setPaused(true);
    }
    if (iterator == WHICH_STEP_TO_WAIT_FOR_PUNCH + 1) {
        m_character->setHealthPercentage(m_healthPostAttack);
    }
    if (iterator == m_timeline->endFrame()) {
        emit finished();
    }
}

void FightDefendingAnimation::attackerPunchedMe() { m_timeline->resume(); }

FightDefendingAnimation::FightDefendingAnimation(CharacterItem* character, float healthPostAttack)
    : FightAnimation(character, AnimationType::Defending)
    , m_healthPostAttack{healthPostAttack} {}
