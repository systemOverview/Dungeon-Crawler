//
// Created by MBK on 13.11.25.
//

#include "Character.h"
#include "Level.h"
#include <qDebug>

Character::Character(CharacterType characterType, Tile* tile)
    : m_characterType(characterType)
    , m_currentTile(tile)

{}

Character* Character::GenerateCharacter(char texture, Tile* tile) {
    switch (texture) {
    case 'P':
        return new Human(CharacterType::Human, tile);
    case 'S':
        return new Zombie(CharacterType::StationaryZombie, tile);
    case 'G':
        return new Zombie(CharacterType::GuardZombie, tile);
    case 'A':
        return new Attacker(CharacterType::Attacker, tile);
    default:
        assert(false && "Character type not handled at Character factory.");
    }
}



Tile* Character::getTile() const { return m_currentTile; }

void Character::setTile(Tile *newTile)
{
    Coordinates preMoveTileCoords = m_currentTile->getCoordinates();
    m_currentTile = newTile;
    emit moved(preMoveTileCoords, newTile->getCoordinates());
}

int Character::getCurrentHP() const { return m_attributes.healthPoints; }

int Character::getStrength() const { return m_attributes.strength; }

bool Character::isAlive() const { return m_attributes.healthPoints > 0; }

Character::CharacterType Character::getCharacterType() const { return m_characterType; }

void to_json(json &jsonObject, const Character *characterObject){
    jsonObject = json {
             {"row", characterObject->getTile()->getRow()},
             {"column", characterObject->getTile()->getColumn()},
             {"HP", characterObject->getCurrentHP()}
    };
}

Character::~Character() {}
