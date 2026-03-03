#include "MoveAnimation.h"
#include "CharacterItem.h"
#include "CharacterTile_UI_PlacementMediator.h"
#include "Constants.h"
MoveAnimation::MoveAnimation(CharacterItem* character,
                             Coordinates fromTileCoords,
                             Coordinates toTileCoords)
    : CharacterAnimation(character, AnimationType::Walking)
    , m_fromTileCoordinates{fromTileCoords}
    , m_toTileCoordinates{toTileCoords} {
    m_timeline = new QTimeLine();
}
void MoveAnimation::start() {
    m_timeline->setFrameRange(0, m_animationFrames.size() - 1);
    int frameCount = (m_timeline->endFrame() - m_timeline->startFrame() + 1);

    float durationInSeconds = frameCount / float(GameSettings::FRAMES_PER_SECOND);

    Coordinates xyAdvancePerFrame = {m_toTileCoordinates.row - m_fromTileCoordinates.row,
                                     m_toTileCoordinates.column - m_fromTileCoordinates.column};
    m_timeline->setDuration(durationInSeconds * 1000);
    if ((durationInSeconds < (0.1))) {
        m_timeline->setDuration(1000);
    }
    m_timeline->setCurrentTime(
        m_timeline
            ->duration()); // Workaround for a known unfixed bug https://qt-project.atlassian.net/browse/QTBUG-41610

    connect(m_timeline, &QTimeLine::frameChanged, [this, xyAdvancePerFrame]() {
        playFrame(m_animationFrames, m_timeline->currentFrame(), xyAdvancePerFrame);
    });
    m_timeline->start();
};
void MoveAnimation::playFrame(std::vector<int> frames, int iterator, Coordinates xyAdvancePerFrame) {
    m_character->setCurrentFrameID(frames.at(iterator));
    CharacterTile_UI_PlacementMediator::AdvanceCharacter(m_character, xyAdvancePerFrame);
    if (iterator == m_timeline->endFrame()) {
        delete m_timeline;
        m_timeline = new QTimeLine();
        emit finished();
    }
}
