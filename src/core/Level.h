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
class Level : public QObject
{
    Q_OBJECT
    std::vector<std::vector<Tile*>> m_tiles = {};
    std::vector<Character*> m_characters = {};
    int m_numberOfRows = 10;
    int m_numberOfColumns = 10;

public: //static functions
    Tile* getTile(int row, int col);
    Tile* getTile(Coordinates coordinates);
    Character* whoIsOccupyingTile(Tile* tile);
signals:
    void tileReplaced(Coordinates coordinatesOfReplacedTile, Tile* newTile);

public:
    Level(std::string gameBoard,
          int numberOfRows = GameSettings::TILES_PER_SIDE,
          int numberOfColumns = GameSettings::TILES_PER_SIDE);

    Level(int numberOfRows = GameSettings::TILES_PER_SIDE,
          int numberOfColumns = GameSettings::TILES_PER_SIDE);

    const std::vector<std::vector<Tile*>> getTiles() const;
    const std::vector<Character*> getCharacters() const;

    void insertOrReplaceTile(Types::TileType tileType, Coordinates tileCoordinates);

    void setDefaultTiles();
    void clear();
    ~Level();
};

#endif //PRAK_LEVEL_H
