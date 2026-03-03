#ifndef FIGHTANIMATION_H
#define FIGHTANIMATION_H

#include <QtCore/qobject.h>
#include "CharacterAnimation.h"
class QState;
class QTimeLine;
class FightAnimation : public CharacterAnimation
{
    Q_OBJECT

public:
    void start() override;
    ;
    enum class FightAnimationState { NotStarted, Preparing, Action };
    enum class FightMode { Defending, Attacking };

private:
    FightAnimationState m_state = FightAnimationState::NotStarted;

protected:
    virtual void playFrame(int iterator) = 0;

public:
    // mediator : AnimatingFightPreparation / AnimatingFightAttack / Restoring
    // states : prepareForAction/doAction/restoreDefault
    // Update :
    // attacker doAction :

    // GUI changes state of involved characters to fighting.
    // GUI alerts both characters animations to prepare.
    // Animations prepare and emit a signal.
    // GUI connects preparation end to attacker punching state.
    // Attacker alerts GUI of punching end.
    // GUI alerts defender to doAction.
    // defender alerts GUI of action end.
    // GUI alerts both to finish up.
    FightAnimation(CharacterItem* character, AnimationType animationType);

    void animateFightPreparation();
    void animateFightAction();
    void restoreToDefault();
};

// **** Attacking animation **** //

class FightAttackingAnimation : public FightAnimation
{
    Q_OBJECT
private:
    static inline constexpr int WHICH_STEP_TO_PUNCH = 2;
    void playFrame(int iterator) override;
signals:
    void iPunchedDefender();

public:
    FightAttackingAnimation(CharacterItem* character);
};

// **** Defending animation **** //

class FightDefendingAnimation : public FightAnimation
{
    Q_OBJECT
private:
    static inline constexpr int WHICH_STEP_TO_WAIT_FOR_PUNCH = 0;
    float m_healthPostAttack{};
    void playFrame(int iterator) override;
public slots:
    void attackerPunchedMe();

public:
    FightDefendingAnimation(CharacterItem* character, float healthPostAttack);
};

#endif // FIGHTANIMATION_H
