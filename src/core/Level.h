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

    inline static std::vector<Character*> CHARACTERS = {};
    int m_gameHeight = 10;
    int m_gameWidth = 10;

public: //static functions
    static Tile* GetTile(int row, int col);
    static Tile* GetTile(Coordinates coordinates);
    static Character* WhoIsOccupyingTile(Tile* tile);

public:
    Level(std::string gameBoard);
    const std::vector<std::vector<Tile*>> getTiles() const;
    const std::vector<Character*> getCharacters() const;

    int getHeight() const;
    int getWidth() const;
    void setDefaultTiles();
    ~Level();
};

#endif //PRAK_LEVEL_H
