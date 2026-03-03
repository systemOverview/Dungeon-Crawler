//
// Created by MBK on 13.11.25.
//

#ifndef PRAK_LEVEL_H
#define PRAK_LEVEL_H
#include "tile.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;
class Character;
class Tile;
class Level
{
    inline static std::vector<std::vector<Tile*>> TILES = {};
    std::vector<Character *> m_characters;
    int m_gameHeight = 10;
    int m_gameWidth = 10;

public: //static functions
    static Tile* GetTile(int row, int col);
    static Tile* GetTile(Coordinates coordinates);

public:
    Level(std::string gameBoard);
    const std::vector<std::vector<Tile*>> getTiles() const;
    int getHeight() const;
    int getWidth() const;
    void setDefaultTiles();
    ~Level();

};

#endif //PRAK_LEVEL_H
