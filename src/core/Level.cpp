//
// Created by MBK on 13.11.25.
//

#include "Level.h"
#include "AttackController.h"
#include "DungeonCrawler.h"
#include <qDebug>

Level::Level(std::string gameString, int numberOfRows, int numberOfColumns)
    : m_numberOfRows{numberOfRows}
    , m_numberOfColumns{numberOfColumns} {
    setDefaultTiles();
    for (int i = 0; i < gameString.length(); i++) {
        int row = i / 10;
        int column = i % 10;
        if (gameString[i] == 'H' || gameString[i] == 'S' || gameString[i] == 'G'
            || gameString[i] == 'A') {
            Tile* tile = Tile::GenerateTile(Types::TileType::Floor, row, column);
            Character* character = Character::GenerateCharacter(gameString[i], tile);
            (m_tiles)[row][column] = tile;
            m_characters.push_back(character);
            continue;
        }

        Tile* tile = Tile::GenerateTile(TypesIdentifiers::CHAR_TO_TILE_TYPE_LOOKUP_TABLE.at(
                                            gameString[i]),
                                        row,
                                        column);
        (m_tiles)[row][column] = tile;
    }
}

Level::Level(int numberOfRows, int numberOfColumns)
    : m_numberOfRows(numberOfRows)
    , m_numberOfColumns(numberOfColumns) {
    m_tiles.reserve(numberOfRows);
    setDefaultTiles();
}

Tile* Level::getTile(int row, int col) {
    Tile* tileToReturn = m_tiles[row][col];
    return tileToReturn;
}

Tile* Level::getTile(Coordinates coordinates) {
    return getTile(coordinates.row, coordinates.column);
}
Character* Level::whoIsOccupyingTile(Tile* tile) {
    for (Character* character : m_characters) {
        if (character->getTile() == tile) {
            return character;
        }
    }
    return nullptr;
}

const std::vector<std::vector<Tile*> > Level::getTiles() const { return m_tiles; }
const std::vector<Character*> Level::getCharacters() const { return m_characters; }

void Level::insertOrReplaceTile(Types::TileType tileType, Coordinates tileCoordinates) {
    Tile* currentTile = m_tiles[tileCoordinates.row][tileCoordinates.column];

    Tile* newTile = Tile::GenerateTile(tileType, tileCoordinates);
    m_tiles[tileCoordinates.row][tileCoordinates.column] = newTile;

    if (currentTile != nullptr) {
        emit tileReplaced(currentTile->getCoordinates(), newTile);
        delete currentTile;
        currentTile = nullptr;
    }
}

void Level::setDefaultTiles()
{
    clear();
    m_tiles.reserve(m_numberOfRows);

    for (int i = 0; i < m_numberOfRows; i++) {
        std::vector<Tile*> row;
        m_tiles.push_back(row);
        for (int j = 0; j < m_numberOfColumns; j++) {
            m_tiles[i].push_back(nullptr);
            insertOrReplaceTile(Types::TileType::Floor, {i, j});
        }
    }
}

void Level::clear() {
    for (int row = 0; row < m_tiles.size(); row++) {
        for (int col = 0; col < m_tiles[row].size(); col++) {
            if (m_tiles[row][col] != nullptr) {
                delete m_tiles[row][col];
                m_tiles[row][col] = nullptr;
            }
        }
    }
    m_tiles.clear();
}

Level::~Level() {}
