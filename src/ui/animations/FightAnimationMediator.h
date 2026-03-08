#ifndef FIGHTANIMATIONMEDIATOR_H
#define FIGHTANIMATIONMEDIATOR_H

#include <QObject>
#include "CharacterItem.h"
class CharacterItem;
class FightAttackingAnimation;
class FightDefendingAnimation;
class FightEvent;
class FightRound;

class FightAnimationMediator : public QObject
{
    Q_OBJECT
private:
    CharacterItem* m_attacker = nullptr;
    CharacterItem* m_defender = nullptr;

    FightAttackingAnimation* m_attackerAnimation = nullptr;
    FightDefendingAnimation* m_defenderAnimation = nullptr;

    FightEvent* m_fightEvent = nullptr;

    qreal m_defenderHealthPostRound{};

    int m_numberOfFinishedAnimations{};

    int m_currentRoundNumber{0};

    void animateRound(FightRound fightRound);

    QMap<int, CharacterItem*> m_fightingCharacters;

public slots:
    void anAnimationWasFinished();
signals:
    void fightRoundAnimationFinished();

public:
    FightAnimationMediator(CharacterItem* attacker,
                           CharacterItem* defender,
                           qreal defenderHealthPostRound,
                           QObject* parent = nullptr);
    FightAnimationMediator(FightEvent* fightEvent, QMap<int, CharacterItem*> fightingCharacters);
    void play();
    void start();

signals:
};

#endif // FIGHTANIMATIONMEDIATOR_H
