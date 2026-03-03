#include "FightEvent.h"

// Start of FightRound definitions.

FightRound::FightRound(const FightRound::CharacterInfo& attackingCharacterInfo,
                       const FightRound::CharacterInfo& defendingCharacterInfo,
                       FightRound::FightRoundOutcome fightRoundOutcome)
    : m_attackingCharacterInfo(attackingCharacterInfo)
    , m_defendingCharacterInfo(defendingCharacterInfo)
    , m_fightRoundOutcome(fightRoundOutcome) {}

FightRound::CharacterInfo FightRound::getAttackingCharacterInfo() const {
    return m_attackingCharacterInfo;
}

FightRound::CharacterInfo FightRound::getDefendingCharacterInfo() const {
    return m_defendingCharacterInfo;
}

FightRound::FightRoundOutcome FightRound::getFightRoundOutcome() const {
    return m_fightRoundOutcome;
}

FightRound::CharacterInfo::CharacterInfo(int characterId, int healthPostRound)
    : m_characterID{characterId}
    , m_healthPostRound{healthPostRound} {}

int FightRound::CharacterInfo::getCharacterID() const { return m_characterID; }

int FightRound::CharacterInfo::getHealthPostRound() const { return m_healthPostRound; }

// Start of FightEvent definitions.
FightEvent::FightEvent()
    : QEvent(type()) {}

QEvent::Type FightEvent::type() {
    if (EVENT_TYPE == None) {
        EVENT_TYPE = static_cast<QEvent::Type>(registerEventType());
    }
    return EVENT_TYPE;
}

void FightEvent::addFightRound(FightRound fightRound) { m_fightRounds.push_back(fightRound); }

std::vector<FightRound> FightEvent::getFightRounds() const { return m_fightRounds; }
