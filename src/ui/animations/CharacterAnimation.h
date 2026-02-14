#ifndef CHARACTERANIMATION_H
#define CHARACTERANIMATION_H

#include <QAbstractAnimation>
#include <QObject>
#include <QPixmap>
#include <QtCore/qtimeline.h>
#include "SpriteManager.h"
class CharacterItem;
class CharacterAnimation : public QAbstractAnimation
{
    Q_OBJECT
protected:
    void updateCurrentTime(int currentTime) override;

public:
    enum class AnimationType { Idle, Walk, Jump, Punch, PAST_END };

private:
    int m_duration = -1; //Infinite loop, the timeline object controls the state of this animation
    CharacterItem* m_character;
    std::pair<AnimationType, int> m_currentAnimationState = {};
    QTimeLine* m_timeLine;
    inline static const std::map<AnimationType, std::pair<int, int>> ANIMATION_FRAMES
        = {//inclusive range
           {AnimationType::Idle, {0, 0}},
           {AnimationType::Walk, {1, 6}},
           {AnimationType::Jump, {16, 18}},
           {AnimationType::Punch, {25, 26}}};
    void setCurrentAnimation(AnimationType animationType);

public slots:
    void playFrame(int frameNumber);

public:
    explicit CharacterAnimation(CharacterItem* character,
                                AnimationType animationType = AnimationType::Idle,
                                QObject* parent = nullptr);
    int duration() const override;
    void setDuration(int duration);
    void begin();

    // QAbstractAnimation interface
protected:
    void updateState(State newState, State oldState) override;
};

#endif // CHARACTERANIMATION_H
