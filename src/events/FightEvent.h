#ifndef FIGHTEVENT_H
#define FIGHTEVENT_H

#include <QEvent>
#include <QObject>

class FightRound
{
public:
    enum class FightRoundOutcome { DefenderKilled, Draw };

    struct CharacterInfo
    {
    private:
        int m_characterID = 0;
        int m_healthPostRound = 0;

    public:
        CharacterInfo(int characterId, int healthPostRound);
        int getCharacterID() const;
        int getHealthPostRound() const;
    };

private:
    CharacterInfo m_attackingCharacterInfo;
    CharacterInfo m_defendingCharacterInfo;
    FightRoundOutcome m_fightRoundOutcome{};

public:
    FightRound(const CharacterInfo& attackingCharacterInfo,
               const CharacterInfo& defendingCharacterInfo,
               FightRoundOutcome fightRoundOutcome);
    CharacterInfo getAttackingCharacterInfo() const;
    CharacterInfo getDefendingCharacterInfo() const;
    FightRoundOutcome getFightRoundOutcome() const;
};

class FightEvent : public QObject, public QEvent
{
    Q_OBJECT
public:

private:
    inline static QEvent::Type EVENT_TYPE = None;
    std::vector<FightRound> m_fightRounds;

public:
    FightEvent();
    static QEvent::Type type();

    void addFightRound(FightRound fightRound);
    std::vector<FightRound> getFightRounds() const;
};

#endif // FIGHTEVENT_H
