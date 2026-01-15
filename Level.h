//
// Created by MBK on 13.11.25.
//

#ifndef PRAK_LEVEL_H
#define PRAK_LEVEL_H
#include "Tile.h"
#include <iostream>
class Character;
class Tile;
class Level
{
    std::vector<std::vector<Tile *>> tiles;
    std::vector<Character *> characters;
    std::vector<Character *> nonPlayableCharacters;
    Character *playableCharacter;
    std::pair <int,int> m_playingCharacterPosition;
    int gameHeight{};
    int gameWidth{};

public:
    Level(int height, int width, std::string gameBoard);

    Tile *getTile(int row, int col);
    std::vector<std::vector<Tile *>> *getTiles();
    int getHeight() const;
    int getWidth() const;

    void placeCharacter(Character *c, int row, int col, bool isPlayable);
    void placePlayingCharacter(Character*c);
    Character *getPlayableCharacter();
    std::vector<Character *> getNonPlayableCharacters();
    void setPortals();
    void setDefaultTiles();
};

#endif //PRAK_LEVEL_H
