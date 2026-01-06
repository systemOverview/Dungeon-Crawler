//
// Created by MBK on 13.11.25.
//

#include "header.h"
#include "Character.h"
#include "Level.h"
#include <qDebug>


void Character::setController(AbstractController *controller)
{
    m_controller = controller;

}

std::string_view Character::getTexture () const {
    return m_texture;
}

std::string_view Character::getTexturePath() const
{
    return m_texturePath;
}

bool Character::isHuman()
{
    return m_isHuman;
}

AbstractController *Character::getController()
{
    return m_controller;
}

Tile * Character::getTile() const {
    return currentTile;
}

void Character::setTile(Tile* newTile) {
    currentTile= newTile;
}

std::pair<int,int> Character::move() {
    return m_controller->move();


}

int Character::getMaxHP()
{
    return (20+m_stamina*5);
}

int Character::getCurrentHP()
{
    return m_hitPoints;
}

bool Character::isAlive()
{
    return m_hitPoints>0;
}


void Character::decrementFromHP(int amountToDecrement)
{
    m_hitPoints-=amountToDecrement;
}

void Character::attackPlayer(Character* characterToAttack)
{
    characterToAttack->decrementFromHP(m_strength);
    qDebug()<<characterToAttack->getTexture() << " was hit and his new HP is " << characterToAttack->getCurrentHP();
}
