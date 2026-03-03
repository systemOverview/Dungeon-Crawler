#include "CharacterAnimation.h"
#include <QtCore/qtimeline.h>
CharacterAnimation::Frames::Frames(int from, int to) {
    for (int i = from; i <= to; i++) {
        m_frames.push_back(i);
    }
}

CharacterAnimation::Frames::Frames(std::vector<int> frames)
    : m_frames{frames} {};
std::vector<int> CharacterAnimation::Frames::getFrames() { return m_frames; };

CharacterAnimation::CharacterAnimation(CharacterItem* character, AnimationType animationType)
    : m_character{character}
    , m_animationFrames{ANIMATION_FRAMES.at(animationType).getFrames()} {
    m_timeline = new QTimeLine;
}

CharacterAnimation::~CharacterAnimation() { delete m_timeline; }
