//
// Created by MBK on 13.11.25.
//

#ifndef PRAK_LEVEL_H
#define PRAK_LEVEL_H
#include <iostream>
#include "Tile.h"
class Character;
class Tile;
class Level {
    std::vector <std::vector<Tile*>> tiles;
    std::vector <Character*> characters;
    Character* playableCharacter;
    int gameHeight{};
    int gameWidth{};
public:
    Level(int height, int width, std::string gameBoard);

    Tile* getTile(int row, int col);
    std::vector <std::vector<Tile*>>* getTiles();
    int getHeight() const;
    int getWidth() const;
    void placeCharacter (Character* c, int row, int col, bool isPlayable);
    Character* getPlayableCharacter() {
        return playableCharacter;
    }
    void setDefaultTiles() {
        for (int i=0; i<gameHeight; i++) {
            std::vector<Tile*> row;
            tiles.push_back(row);
            for (int j=0; j<gameWidth;j++) {
                tiles[i].push_back(nullptr);
            }
        }
    }



};


#endif //PRAK_LEVEL_H
