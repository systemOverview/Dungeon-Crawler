#ifndef LOOPINGANIMATION_H
#define LOOPINGANIMATION_H
#include "CharacterAnimation.h"
class LoopingAnimation : public CharacterAnimation
{
private:
    AnimationType m_animationType = AnimationType::Looping;

public:
    LoopingAnimation(CharacterItem* character);
    void start() override;
    void playFrame(int iterator);
};

#endif // LOOPINGANIMATION_H
