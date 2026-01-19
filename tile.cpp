//
// Created by MBK on 12.11.25.
//

#include "Tile.h"
#include <QtCore/qdebug.h>

std::string_view Tile::getTexture() const
{
    return texture;
}

bool Tile::hasCharacter() const
{
    return (character != nullptr);
}

Character *Tile::getCharacter() const
{
    return character;
}

void Tile::setCharacter(Character *characterToPlace)
{
    character = characterToPlace;
    EventBus::transmitEvent<EventBus::TileChange>(this, TileChangeEvent::Character);
}

int Tile::getRow()
{
    return row;
}

int Tile::getColumn()
{
    return column;
}

std::pair<int, int> Tile::getCordsAsPair()
{
    return {row, column};
}

bool Tile::moveTo(Tile *desTile, Character *who)
{
    if (onLeave(desTile, who)) {
        std::pair<bool, Tile *> onEnterResult = desTile->onEnter(who);
        if (onEnterResult.first == false) {
            return false;
        }

        Tile *tileToMoveTo;

        if (onEnterResult.second != nullptr) {
            tileToMoveTo= onEnterResult.second; // if onEnter returns a non-nullptr as it's second argument, then it is the portal that the player will access
        } else {
            tileToMoveTo= desTile; // if onEnter returns a nullptr as it's second argument, then the desTile is a normal tile to be accessed
        }
        if (tileToMoveTo->hasCharacter()) {
            Character *characterAtWantedTile = tileToMoveTo->getCharacter();
            if (characterAtWantedTile->isHuman()!= who->isHuman()) { // then it means they are a human and a zombie, which means that they can fight
                who->attackPlayer(characterAtWantedTile);
                if (characterAtWantedTile->isAlive()) {
                    characterAtWantedTile->attackPlayer(who);

                } else {
                    who->setTile(tileToMoveTo);
                    tileToMoveTo->setCharacter(who);
                    setCharacter(nullptr);
                }
            }
        } else {
            who->setTile(tileToMoveTo);
            tileToMoveTo->setCharacter(who);
            setCharacter(nullptr);
        }
        return true;
    }
    return false;
}

bool Tile::onLeave(Tile *desTile, Character *who)
{
    return true;
}

std::pair<bool, Tile *> Tile::onEnter(Character *who)
{
    std::pair<bool, Tile *> result(isEntrable(), nullptr);
    return result;
}

std::string Tile::getTexturePath()
{
    return m_texturePath;
}

std::pair<bool, Tile *> Portal::onEnter(Character *who)
{
    std::pair<bool, Tile *> result(isEntrable(), portalToAccess);
    return result;
}

std::pair<bool, Tile *> Switch::onEnter(Character *who)
{
    activate();
    std::pair<bool, Tile *> result(isEntrable(), nullptr);
    return result;
}

/*door*/
void Door::notify()
{
    state = !state;
    if (state) {
        texture = "/";
        setTexturePath(":/pics/textures/doors/door2.png");

    } else {
        texture = "X";
        setTexturePath(":/pics/textures/doors/door1.png");
    }
    EventBus::transmitEvent<EventBus::TileChange>(this, TileChangeEvent::DoorStatus);

}

/*pit*/

bool Pit::onLeave(Tile *desTile, Character *who)
{
    if (desTile->getTexture() != "<" && desTile->getTexture() != "_") {
        return false;
    }
    return true;
}

std::string Wall::getTexturePath()
{
    return m_texturePath;
}
