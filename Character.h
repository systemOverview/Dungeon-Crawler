//
// Created by MBK on 13.11.25.
//

#ifndef PRAK_CHARACTER_H
#define PRAK_CHARACTER_H
#include "TerminalUI.h"
#include "AbstractUI.h"
#include <iostream>
class Tile;
class TerminalUI;
class Character{
private:
    std::string Texture;
    int stength{};
    int stamina{};
    int hitPoints{};
    Tile* currentTile; // The tile the player is at
    AbstractController* controller = new TerminalUI;
public:
    Character(std::string_view txt, Tile* tile=nullptr) : Texture (txt),
    currentTile (tile) {

    };
    AbstractUI* getTerminal();

    std::string_view getTexture() const;
    Tile* getTile() const;
    void setTile(Tile* newTile);

    int move(Level *currentLevel);



};



#endif //PRAK_CHARACTER_H
