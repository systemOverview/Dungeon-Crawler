//
// Created by MBK on 13.11.25.
//

#include "Character.h"
#include "Level.h"
#include "QCharacter.h"
#include <qDebug>

Character* Character::GenerateCharacter(char texture, int hitPoints,  Tile* tile, Level* level , LevelGraph* levelGraph)
{
    switch (texture){
    case 'P' : return new Character(texture, 20, 20, tile, hitPoints);
    case 'S' : return new Zombie(texture, tile, hitPoints);
    case 'G' : return new Zombie(texture,tile, hitPoints);
    case 'A' : return new Attacker(texture,  level, levelGraph,tile, hitPoints);
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

Tile *Character::getTile() const
{
    return currentTile;
}

void Character::setTile(Tile *newTile)
{
    currentTile = newTile;
}

std::pair<int, int> Character::move()
{
    return m_controller->move();
}

int Character::getMaxHP() const
{
    return (20 + m_stamina * 5);
}

int Character::getCurrentHP() const
{
    return m_hitPoints;
}

int Character::getStrength() const {return m_strength;}

bool Character::isAlive() const
{
    return m_hitPoints > 0;
}

void Character::decrementFromHP(int amountToDecrement)
{
    m_hitPoints -= amountToDecrement;
    EventBus::transmitEvent<EventBus::CharacterHealthChange>(this);

}

void Character::attackPlayer(Character *characterToAttack)
{
    characterToAttack->decrementFromHP(m_strength);
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
