#include "FightAnimationMediator.h"

#include "CharacterItem.h"
#include "FightAnimation.h"
#include "FightEvent.h"

FightAnimationMediator::FightAnimationMediator(CharacterItem* attacker,
                                               CharacterItem* defender,
                                               qreal defenderHealthPostRound,
                                               QObject* parent)
    : QObject(parent)
    , m_attacker(attacker)
    , m_defender(defender)
    , m_defenderHealthPostRound{defenderHealthPostRound} {}

FightAnimationMediator::FightAnimationMediator(FightEvent* fightEvent,
                                               QMap<int, CharacterItem*> fightingCharacters)
    : m_fightEvent{fightEvent}
    , m_fightingCharacters{fightingCharacters} {}

void FightAnimationMediator::play() {}

void FightAnimationMediator::start() { animateRound(m_fightEvent->getFightRounds().at(0)); }

void FightAnimationMediator::animateRound(FightRound fightRound) {
    CharacterItem* attackerView = *(
        m_fightingCharacters.find(fightRound.getAttackingCharacterInfo().getCharacterID()));
    CharacterItem* defenderView = *(
        m_fightingCharacters.find(fightRound.getDefendingCharacterInfo().getCharacterID()));

    FightAttackingAnimation* attackerAnimation = new FightAttackingAnimation(attackerView);

    FightDefendingAnimation* defenderAnimation
        = new FightDefendingAnimation(defenderView,
                                      fightRound.getDefendingCharacterInfo().getHealthPostRound());

    defenderView->lookTowardsPosition(attackerView->pos());

    connect(attackerAnimation,
            &FightAttackingAnimation::iPunchedDefender,
            defenderAnimation,
            &FightDefendingAnimation::attackerPunchedMe);

    connect(attackerAnimation,
            &CharacterAnimation::finished,
            this,
            &FightAnimationMediator::anAnimationWasFinished);
    connect(defenderAnimation,
            &CharacterAnimation::finished,
            this,
            &FightAnimationMediator::anAnimationWasFinished);

    attackerView->addAnimationToQueue(attackerAnimation);
    defenderView->addAnimationToQueue(defenderAnimation);

    return;
}

void FightAnimationMediator::anAnimationWasFinished() {
    m_numberOfFinishedAnimations++;
    if (m_numberOfFinishedAnimations == 2) {
        m_currentRoundNumber++;
        if (m_currentRoundNumber < m_fightEvent->getFightRounds().size()) {
            m_numberOfFinishedAnimations = 0;
            animateRound(m_fightEvent->getFightRounds().at(m_currentRoundNumber));
        }
        else {
            emit fightRoundAnimationFinished();
        }
    }
}
