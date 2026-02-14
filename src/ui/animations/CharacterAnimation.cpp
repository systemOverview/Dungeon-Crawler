#include "CharacterAnimation.h"
#include <QDebug>
#include <QtCore/qtimeline.h>
#include <QtCore/qtimer.h>
#include "CharacterItem.h"
void CharacterAnimation::updateCurrentTime(int currentTime) {}

void CharacterAnimation::playFrame(int frameId) {
    m_character->setCurrentFrameId(frameId);
    // m_character->setPos(m_character->pos().x() + 10, m_character->pos().y());
}

CharacterAnimation::CharacterAnimation(CharacterItem* character,
                                       AnimationType animationType,
                                       QObject* parent)
    : m_character{character}
    , QAbstractAnimation(parent) {
    setCurrentAnimation(animationType);
}

void CharacterAnimation::setCurrentAnimation(AnimationType animationType) {
    m_timeLine = new QTimeLine();
    m_timeLine->setFrameRange(ANIMATION_FRAMES.at(animationType).first,
                              ANIMATION_FRAMES.at(animationType).second);
    int frameCount = (m_timeLine->endFrame() - m_timeLine->startFrame());
    float durationInSeconds = frameCount / float(GameSettings::FPS);

    m_timeLine->setDuration(durationInSeconds * 1000);
    if ((durationInSeconds < (0.1))) {
        m_timeLine->setDuration(1000);
    }
    m_timeLine->setCurrentTime(
        m_timeLine
            ->duration()); // Workaround for a known unfixed bug https://qt-project.atlassian.net/browse/QTBUG-41610

    connect(m_timeLine, &QTimeLine::frameChanged, this, &CharacterAnimation::playFrame);
    connect(m_timeLine, &QTimeLine::finished, this, &CharacterAnimation::finished);
}

int CharacterAnimation::duration() const { return m_duration; }

void CharacterAnimation::setDuration(int duration) { m_duration = duration; }

void CharacterAnimation::begin() {}

void CharacterAnimation::updateState(State newState, State oldState) {
    if (newState == QAbstractAnimation::Stopped) {
        m_currentAnimationState.second = ANIMATION_FRAMES.at(m_currentAnimationState.first).first;
    }
    else if (newState == QAbstractAnimation::Running) {
        if (oldState == QAbstractAnimation::Paused) {
            m_timeLine->resume();
        }
        else {
            m_timeLine->start();
        }
    }
    else {
        m_timeLine->setPaused(true);
    }
    QAbstractAnimation::updateState(newState, oldState);
}
