//
// Created by MBK on 13.11.25.
//

#include "Level.h"
#include "AttackController.h"
#include "DungeonCrawler.h"
#include <qDebug>

Level::Level(std::string gameString) {
    setDefaultTiles();
    for (int i = 0; i < gameString.length(); i++) {
        int row = i / 10;
        int column = i % 10;
        Tile* tile = Tile::GenerateTile(gameString[i], row, column);
        (TILES)[row][column] = tile;

        if (gameString[i] == 'H' || gameString[i] == 'S' || gameString[i] == 'G'
            || gameString[i] == 'A') {
            DungeonCrawler::CreateCharacter(gameString[i], tile);
        }
    }
}

Tile* Level::GetTile(int row, int col) {
    Tile* tileToReturn = TILES[row][col];
    return tileToReturn;
}

Tile* Level::GetTile(Coordinates coordinates) {
    return GetTile(coordinates.row, coordinates.column);
}

const std::vector<std::vector<Tile*> > Level::getTiles() const { return TILES; }

int Level::getHeight() const
{
    return m_gameHeight;
}

int Level::getWidth() const
{
    return m_gameWidth;
}

void Level::setDefaultTiles()
{
    for (int i = 0; i < GameSettings::TILES_PER_SIDE; i++) {
        std::vector<Tile *> row;
        TILES.push_back(row);
        for (int j = 0; j < GameSettings::TILES_PER_SIDE; j++) {
            TILES[i].push_back(nullptr);
        }
    }
}


Level::~Level()
{
}
