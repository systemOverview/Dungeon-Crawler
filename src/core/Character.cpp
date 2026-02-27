//
// Created by MBK on 13.11.25.
//

#include "Character.h"
#include "Level.h"
#include "QCharacter.h"
#include <qDebug>

Character* Character::GenerateCharacter(char texture,
                                        Tile* tile,
                                        Level* level,
                                        LevelGraph* levelGraph) {
    switch (texture) {
    case 'P':
        return new Human(texture, tile);
    case 'S':
        return new Zombie(texture, tile);
    case 'G':
        return new Zombie(texture, tile);
    case 'A':
        return new Attacker(texture, level, levelGraph, tile);
    default : throw std::logic_error("Character type not handled at Character factory.");
    }
}

void Character::setController(AbstractController *controller)
{
    m_controller = controller;
}

char Character::getTexture() const
{
    return m_texture;
}

std::string Character::getTexturePath() const
{
    return m_texturePath;
}

bool Character::isHuman() const
{
    return m_texture=='P';
}

AbstractController *Character::getController() const
{
    return m_controller;
}

Tile* Character::getTile() const { return m_currentTile; }

void Character::setTile(Tile *newTile)
{
    Coordinates preMoveTileCoords = m_currentTile->getCoordinates();
    m_currentTile = newTile;
    emit moved(preMoveTileCoords, newTile->getCoordinates());
}

std::pair<int, int> Character::move() { return m_controller->move(); }

int Character::getMaxHP() const { return m_attributes.stamina; }

int Character::getCurrentHP() const { return m_healthPoints; }

int Character::getStrength() const { return m_attributes.strength; }

bool Character::isAlive() const { return m_healthPoints > 0; }

void Character::decrementFromHP(int amountToDecrement)
{
    m_healthPoints -= amountToDecrement;
    EventBus::transmitEvent<EventBus::CharacterHealthChange>(this);

}

void Character::attackPlayer(Character *characterToAttack)
{
    characterToAttack->decrementFromHP(m_attributes.strength);
    qDebug() << characterToAttack->getTexture() << " was hit and his new HP is "
             << characterToAttack->getCurrentHP();
}

void Character::setQCharacter(QCharacter* QCharacter){
    m_QCharacter = QCharacter;
}

QCharacter* Character::getQChatacter()
{
    return m_QCharacter;
}

Character::~Character()
{
    delete m_controller;
    // EventBus::transmitEvent<EventBus::CharacterHealthChange>(this);
}



void to_json(json &jsonObject, const Character *characterObject){
    jsonObject = json {
             {"texture",  characterObject->getTexture() },
             {"row", characterObject->getTile()->getRow()},
             {"column", characterObject->getTile()->getColumn()},
             {"HP", characterObject->getCurrentHP()}
    };
}
