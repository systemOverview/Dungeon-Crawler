#ifndef CHARACTERANIMATION_H
#define CHARACTERANIMATION_H

#include <QAbstractAnimation>
#include <QObject>
#include <QPixmap>
#include <QtCore/qdebug.h>
#include <QtCore/qtimeline.h>
#include "Constants.h"
#include <CharacterItem.h>
class CharacterAnimation : public QAbstractAnimation
{
    Q_OBJECT

protected:
    bool debug = false; //TEST
    void updateCurrentTime(int currentTime) override;

public:

private:
    int m_duration = -1; //Infinite loop, the timeline object controls the state of this animation
    CharacterItem* m_character;
    QTimeLine* m_timeline;
    std::pair<qreal, qreal> m_advancePerFrame = {0, 0};
    inline static const std::map<CharacterItem::State, std::pair<int, int>> ANIMATION_FRAMES = {
               //inclusive range
               {CharacterItem::State::Idle, {0, 0}},
               {CharacterItem::State::Walk, {1, 6}},
               {CharacterItem::State::Jump, {16, 18}},
               {CharacterItem::State::Punch, {25, 26}},
    };

    void setCurrentAnimation(CharacterItem::State animationType);
    std::vector<int> getAnimationFramesAsVector(CharacterItem::State state) const;
    Coordinates calculateXYadvancePerFrame(Coordinates fromTileCoords, Coordinates ToTileCoords);
    int m_loopCount = -1;

public slots:
    void playFrame(std::vector<int> frames, int iterator, Coordinates xyAdvancePerFrame = {0, 0});
    void updateAnimation(CharacterItem::State characterState);

public:
    explicit CharacterAnimation(CharacterItem* character, QObject* parent = nullptr);
    int duration() const override;
    void setDuration(int duration);
    void loopThroughAll();
    void animateMove(Coordinates fromTileCoords, Coordinates ToTileCoords);

protected:
    void updateState(State newState, State oldState) override;
};

#endif // CHARACTERANIMATION_H
