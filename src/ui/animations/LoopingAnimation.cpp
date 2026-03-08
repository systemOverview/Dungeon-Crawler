#include "LoopingAnimation.h"
#include "CharacterItem.h"
#include "Constants.h"

#include <QtCore/qtimeline.h>
#include <qtimer.h>

LoopingAnimation::LoopingAnimation(CharacterItem* character)
    : CharacterAnimation{character, CharacterAnimation::AnimationType::Looping} {}

void LoopingAnimation::start() {
    int framesCount = m_animationFrames.size();

    m_timeline->setFrameRange(0, framesCount);
    m_timeline->setDuration((framesCount / GameSettings::FRAMES_PER_SECOND) * 1000);

    connect(m_timeline, &QTimeLine::frameChanged, this, &LoopingAnimation::playFrame);
    connect(m_timeline, &QTimeLine::finished, this, [this]() mutable {
        delete m_timeline;
        m_timeline = nullptr;
        m_timeline = new QTimeLine();
        start();
    });
    m_timeline->start();
}

void LoopingAnimation::playFrame(int iterator) {
    m_character->setCurrentFrameID(m_animationFrames[iterator]);
}
