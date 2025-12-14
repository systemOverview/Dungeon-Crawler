//
// Created by MBK on 13.11.25.
//

#include "header.h"
#include "Character.h"

#include "Level.h"



std::string_view Character::getTexture () const {
    return Texture;
}

Tile * Character::getTile() const {
    return currentTile;
}

void Character::setTile(Tile* newTile) {
    currentTile= newTile;
}

int Character::move(Level *currentLevel) {


    int step = controller->move();
    return step;





}
