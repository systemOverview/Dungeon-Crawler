//
// Created by MBK on 13.11.25.
//

#include "Character.h"
#include "Level.h"
#include <qDebug>
#include <qapplication.h>

void Character::setTile(Tile* newTile) {
    Tile* preMoveTile = m_currentTile;
    m_currentTile = newTile;
    newTile->alertOfAccess();
    emit characterMoved(m_characterID,
                        preMoveTile->getCoordinates(),
                        m_currentTile->getCoordinates());

    if (newTile->getTileType() == Types::TileType::Portal
        && preMoveTile->getTileType() != Types::TileType::Portal) {
        Portal* currentPortal = dynamic_cast<Portal*>(newTile);
        setTile(currentPortal->getSiblingPortal());
    }
}

Character::Character(Types::CharacterType characterType,
                     CharactersAttributes::Attributes attributes,
                     Tile* tile)
    : m_characterID{LAST_ASSIGNED_CHARACTER_ID++}
    , m_characterType(characterType)
    , m_attributes(attributes)
    , m_currentTile(tile)

{}

Character* Character::GenerateCharacter(Types::CharacterType characterType, Tile* tile) {
    switch (characterType) {
    case Types::CharacterType::Human:
        return new Human(Types::CharacterType::Human, tile);
    case Types::CharacterType::StationaryZombie:
        return new Zombie(Types::CharacterType::StationaryZombie, tile);
    case Types::CharacterType::GuardZombie:
        return new Zombie(Types::CharacterType::GuardZombie, tile);
    case Types::CharacterType::Attacker:
        return new Attacker(Types::CharacterType::Attacker, tile);
    default:
        assert(false && "Character type not handled at Character factory.");
    }
}

int Character::getCharacterID() const { return m_characterID; }

Types::CharacterType Character::getCharacterType() const { return m_characterType; }

Tile* Character::getTile() const { return m_currentTile; }

int Character::getStrength() const { return m_attributes.strength; }

void Character::decrementFromHealthPoints(int howMuch) { m_attributes.healthPoints -= howMuch; }

int Character::getCurrentHealthPoints() const { return m_attributes.healthPoints; }

bool Character::isAlive() const { return m_attributes.healthPoints > 0; }

void Character::setTileWithoutEmittingSignal(Tile* newTile) {
    m_currentTile = newTile;
    newTile->alertOfAccess();
}

void to_json(json &jsonObject, const Character *characterObject){
    jsonObject = json{{"row", characterObject->getTile()->getRow()},
                      {"column", characterObject->getTile()->getColumn()},
                      {"HP", characterObject->getCurrentHealthPoints()}};
}

Character::~Character() {}
