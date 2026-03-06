//
// Created by MBK on 13.11.25.
//

#ifndef PRAK_CHARACTER_H
#define PRAK_CHARACTER_H
#include <QObject>
#include "Constants.h"
#include "Types.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;
class Tile;
class Character : public QObject
{
    Q_OBJECT
public:
    enum class State { Customizing, Idle, Walking, Defending, Attacking };

protected:
    inline static int LAST_ASSIGNED_CHARACTER_ID = 0;

    Character(Types::CharacterType characterType,
              CharactersAttributes::Attributes attributes,
              Tile* tile = nullptr);

    int m_characterID{};
    Types::CharacterType m_characterType{};
    CharactersAttributes::Attributes m_attributes{};
    Tile* m_currentTile = nullptr;

public slots:
    void setTile(Tile* newTile);
signals:
    void characterMoved(int characterID, Coordinates from, Coordinates to);

public:
    static Character* GenerateCharacter(Types::CharacterType characterType, Tile* tile = nullptr);

    int getCharacterID() const;
    Types::CharacterType getCharacterType() const;

    Tile* getTile() const;

    int getStrength() const;

    int getCurrentHealthPoints() const;
    void decrementFromHealthPoints(int howMuch);

    bool isAlive() const;
    void setTileWithoutEmittingSignal(Tile* newTile);

    virtual ~Character();
};

// Start of subclasses
class Human : public Character
{
public:
    Human(Types::CharacterType characterType, Tile* tile = nullptr)
        : Character(characterType, CharactersAttributes::HumanAttributes, tile) {};
};

class Zombie : public Character
{

public:
    Zombie(Types::CharacterType characterType, Tile* tile = nullptr)
        : Character(characterType, CharactersAttributes::ZombieAttributes, tile) {};
};

class Attacker : public Character
{
public:
    Attacker(Types::CharacterType characterType, Tile* tile = nullptr)
        : Character(characterType, CharactersAttributes::AttackerAttributes, tile) {};
};

void to_json(json &jsonObject, const Character* characterObject);

#endif //PRAK_CHARACTER_H
