#ifndef CHARACTERANIMATION_H
#define CHARACTERANIMATION_H

#include <QObject>
class QTimeLine;
class CharacterItem;

class CharacterAnimation : public QObject
{
    Q_OBJECT
public:
    enum class AnimationType { Idle, Looping, Walking, Defending, Attacking };

protected:
    struct Frames
    {
    private:
        std::vector<int> m_frames;

    public:
        Frames(int from, int to); // constructor for frames using a range
        Frames(std::vector<int> frames); // constructor for frames using a vector
        std::vector<int> getFrames();
    };
    std::vector<int> m_animationFrames;

    CharacterItem* m_character = nullptr;

    QTimeLine* m_timeline = nullptr;

private:
    inline static std::map<AnimationType, Frames> ANIMATION_FRAMES
        = {{AnimationType::Idle, Frames{0, 0}},
           {AnimationType::Looping, Frames{0, 49}},
           {AnimationType::Walking, Frames{1, 6}},
           {AnimationType::Attacking, Frames{20, 23}},
           {AnimationType::Defending, Frames(std::vector{29, 11, 13})}

        };

public:
    CharacterAnimation(CharacterItem* character, AnimationType animationType);
    virtual void start() = 0;
    virtual ~CharacterAnimation();

signals:
    void finished();
};
#endif // CHARACTERANIMATION_H
