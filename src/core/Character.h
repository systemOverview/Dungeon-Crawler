//
// Created by MBK on 13.11.25.
//

#ifndef PRAK_CHARACTER_H
#define PRAK_CHARACTER_H
#include <QObject>
#include "Constants.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class Tile;
class Character : public QObject
{
    Q_OBJECT
public:
    enum class CharacterType { Human, StationaryZombie, GuardZombie, Attacker };

protected:
    CharacterType m_characterType;

    CharactersAttributes::Attributes m_attributes;
    Tile* m_currentTile = nullptr;

public slots:
    void setTile(Tile* newTile);
signals:
    void moved(Coordinates fromTileCoords, Coordinates ToTileCoords);

public:
    Character(CharacterType characterType, Tile* tile = nullptr);
    static Character* GenerateCharacter(char texture, Tile* tile = nullptr);

    Tile* getTile() const;

    int getMaxHP() const;
    int getCurrentHP() const;
    int getStrength() const;
    bool isAlive() const;
    bool isHuman() const;

    CharacterType getCharacterType() const;
    virtual ~Character();
};

// Start of subclasses
class Human : public Character
{
public:
    Human(CharacterType characterType, Tile* tile = nullptr)
        : Character(characterType, tile) {};
};

class Zombie : public Character
{

public:
    Zombie(Character::CharacterType characterType, Tile* tile = nullptr)
        : Character(characterType, tile) {};
};

class Attacker : public Character
{
public:
    Attacker(CharacterType characterType, Tile* tile = nullptr)
        : Character(characterType, tile) {};
};

void to_json(json &jsonObject, const Character* characterObject);

#endif //PRAK_CHARACTER_H
