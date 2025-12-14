//
// Created by MBK on 12.11.25.
//

#include "Tile.h"

std::string_view Tile::getTexture() const {
    if (character) {
        return character->getTexture();
    }
    return texture;
}

bool Tile::hasCharacter() const {
    return (character!=nullptr);
}

Character *Tile::getCharacter() const {
    return character;
}


void Tile::setCharacter(Character *characterToPlace)  {
    character = characterToPlace;
}

int Tile::getRow() {
    return row;
}

int Tile::getColumn() {
    return column;
}

bool Tile::moveTo(Tile *desTile, Character *who) {

    /*
    std::cout << typeid(this).name() << "\n";
    */
    if (onLeave(desTile,who) ) {

        std::pair <bool, Tile*> onEnterResult = desTile->onEnter(who);
        if (onEnterResult.first==false) {
            return false;
        }


        if (onEnterResult.second!=nullptr) {
            character->setTile(onEnterResult.second);
            onEnterResult.second->setCharacter(who);
        }
        else {
            character->setTile(desTile);
            desTile->setCharacter(who);
        }
        setCharacter(nullptr);
        return true;
    }
    return false;
}

bool Tile::onLeave(Tile *desTile, Character *who) {
    return true;
}

std::pair<bool, Tile*> Tile::onEnter(Character *who) {
    std::pair<bool, Tile*> result (isEntrable(), nullptr);
    return result;

}

std::string Tile::getTexturePath()
{
    return texturePath;
}


std::pair<bool, Tile*> Portal::onEnter(Character *who) {
    std::pair<bool, Tile*> result (isEntrable(), portalToAccess);
    return result;
}

std::pair<bool, Tile *> Switch::onEnter(Character *who) {
    activate();
    std::pair<bool, Tile*> result (isEntrable(), nullptr);
    return result;

}

/*door*/
void Door::notify() {
    state = !state;
    if (state) {
        texture = "/";
        texturePath = ":/pics/textures/doors/door2.png";

    }
    else {
        texture ="X";
        texturePath = ":/pics/textures/doors/door1.png";
    }
}

/*pit*/

bool Pit::onLeave(Tile *desTile, Character *who) {
    if (desTile->getTexture() != "<" && desTile->getTexture() != "_") {
        return false;
    }
    return true;
}

std::string Wall::getTexturePath()
{
    return texturePath;
}
