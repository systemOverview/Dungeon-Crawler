#include "CharacterAnimation.h"
#include <QDebug>
#include <QtCore/qtimeline.h>
#include <QtCore/qtimer.h>
#include <QtWidgets/qgraphicsscene.h>
#include "CharacterItem.h"
#include "Constants.h"
#include <CharacterTile_UI_PlacementMediator.h>
void CharacterAnimation::updateCurrentTime(int currentTime) {}

void CharacterAnimation::updateAnimation(CharacterItem::State characterState) {
    delete m_timeline; // stop the current timeline no matter what, we are moving to a new animation
    m_timeline = new QTimeLine();
    if (characterState == CharacterItem::State::Looping) {
        loopThroughAll();
    }
    else {
        setLoopCount(1); // no longer looping forever.
        setCurrentAnimation(characterState);
    }
    start();
}

CharacterAnimation::CharacterAnimation(CharacterItem* character,
                                       QObject* parent)
    : m_character{character}
    , QAbstractAnimation(parent) {
    m_timeline = new QTimeLine();
    setLoopCount(-1);

}

void CharacterAnimation::setCurrentAnimation(CharacterItem::State animationType) {
    if (m_character->getCurrentFrameID() != 0) {
        m_character->setCurrentFrameID(0);
    }
    std::vector<int> frames = getAnimationFramesAsVector(animationType);
    m_timeline->setFrameRange(0, frames.size() - 1);
    int frameCount = (m_timeline->endFrame() - m_timeline->startFrame() + 1);

    float durationInSeconds = frameCount / float(GameSettings::FPS);

    std::pair<qreal, qreal> xyAdvance
        = {(m_character->getNewPosition().x() - m_character->pos().x()) / frameCount,
           (m_character->getNewPosition().y() - m_character->pos().y()) / frameCount};
    m_timeline->setDuration(durationInSeconds * 1000);
    if ((durationInSeconds < (0.1))) {
        m_timeline->setDuration(1000);
    }
    m_timeline->setCurrentTime(
        m_timeline
            ->duration()); // Workaround for a known unfixed bug https://qt-project.atlassian.net/browse/QTBUG-41610

    connect(m_timeline, &QTimeLine::frameChanged, [this, frames, xyAdvance]() {
        playFrame(frames, m_timeline->currentFrame(), xyAdvance);
    });
    m_timeline->start();
}

std::vector<int> CharacterAnimation::getAnimationFramesAsVector(CharacterItem::State state) const {
    std::vector<int> frames;
    if (state == CharacterItem::State::Looping) { // all animations
        for (auto [state, beginEndRange] : ANIMATION_FRAMES) {
            if (state == CharacterItem::State::Idle) continue;
            for (int i = beginEndRange.first; i <= beginEndRange.second; i++) {
                frames.push_back(i); // get all the frames to loop through them
            }
        }
    }
    else {
        std::pair<int, int> animationFramesRange = ANIMATION_FRAMES.at(state);
        for (int frameID = animationFramesRange.first; frameID <= animationFramesRange.second;
             frameID++) {
            frames.push_back(frameID);
        }
    }
    return frames;
}

int CharacterAnimation::duration() const {
    return 100; //TEST
    ;
}

void CharacterAnimation::setDuration(int duration) { m_duration = duration; }

void CharacterAnimation::loopThroughAll() {
    std::vector<int> frames = getAnimationFramesAsVector(CharacterItem::State::Looping);

    m_timeline->setFrameRange(0, frames.size() - 1);
    // m_timeLine->setDuration(frames.size() * GameSettings::FPS * 1000); //TESt
    m_timeline->setDuration(5000);
    connect(m_timeline, &QTimeLine::frameChanged, [this, frames]() {
        playFrame(frames, m_timeline->currentFrame());
    });
    connect(m_timeline, &QTimeLine::finished, m_timeline, &QTimeLine::start);

    m_timeline->start();
}
void CharacterAnimation::playFrame(std::vector<int> frames,
                                   int iterator,
                                   std::pair<qreal, qreal> xyAdvancePerFrame) {
    m_character->setCurrentFrameID(frames.at(iterator));
    CharacterTile_UI_PlacementMediator::AdvanceCharacter(m_character, 1);
}

void CharacterAnimation::updateState(State newState, State oldState) {
}
