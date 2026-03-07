//
// Created by MBK on 13.11.25.
//

#include "Level.h"
#include "AttackController.h"
#include "DungeonCrawler.h"
#include <Utilities.h>
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
            Character* character = Character::GenerateCharacter(
                TypesIdentifiers::CHAR_TO_CHARACTER_TYPE_LOOKUP_TABLE.at(gameString[i]), tile);
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

Level::Level(json levelJson, int numberOfRows, int numberOfColumns) {
    setDefaultTiles();

    for (auto tileJson : levelJson["tiles"]) {
        Types::TileType tileType = Utilities::QString_To_Q_ENUM<Types::TileType>(
            QString::fromStdString(tileJson["type"]));
        int row = tileJson["row"];
        int column = tileJson["column"];

        createAndInsertOrReplaceTile(tileType, {row, column});
    }

    for (auto characterJSON : levelJson["characters"]) {
        Types::CharacterType characterType = Utilities::QString_To_Q_ENUM<Types::CharacterType>(
            QString::fromStdString(characterJSON["type"]));
        int row = characterJSON["row"];
        int column = characterJSON["column"];

        createAndInsertCharacter(characterType, {row, column});
    }

    for (auto portalConnectionJSON : levelJson["portalConnections"]) {
        int firstPortalRow = portalConnectionJSON["firstPortalRow"];
        int firstPortalColumn = portalConnectionJSON["firstPortalColumn"];
        int secondPortalRow = portalConnectionJSON["secondPortalRow"];
        int secondPortalColumn = portalConnectionJSON["firstPortalColumn"];

        connectPortals({firstPortalRow, firstPortalColumn}, {secondPortalRow, secondPortalColumn});
    }
}

void Level::setDefaultTiles() {
    clear();
    m_tiles.reserve(m_numberOfRows);

    for (int i = 0; i < m_numberOfRows; i++) {
        std::vector<Tile*> row;
        m_tiles.push_back(row);
        for (int j = 0; j < m_numberOfColumns; j++) {
            m_tiles[i].push_back(nullptr);
            createAndInsertOrReplaceTile(Types::TileType::Floor, {i, j});
        }
    }
}

Character* Level::whoIsOccupyingTile(Tile* tile) {
    for (Character* character : m_characters) {
        if (character->getTile() == tile) {
            return character;
        }
    }
    return nullptr;
}


void Level::createAndInsertOrReplaceTile(Types::TileType tileType, Coordinates tileCoordinates) {
    Tile* currentTile = m_tiles[tileCoordinates.row][tileCoordinates.column];

    Tile* newTile = Tile::GenerateTile(tileType, tileCoordinates);
    m_tiles[tileCoordinates.row][tileCoordinates.column] = newTile;

    if (currentTile != nullptr) {
        emit tileReplaced(currentTile->getCoordinates(), newTile);
        delete currentTile;
        currentTile = nullptr;
    }
}

Character* Level::createAndInsertCharacter(Types::CharacterType characterType,
                                           Coordinates coordinates) {
    Character* character
        = Character::GenerateCharacter(characterType, m_tiles[coordinates.row][coordinates.column]);

    character->setTile(m_tiles[coordinates.row][coordinates.column]);

    m_characters.push_back(character);

    return character;
}

void Level::connectPortals(Coordinates firstCoordinates, Coordinates secondCoordinates) {
    Portal* firstPortal = dynamic_cast<Portal*>(
        m_tiles[firstCoordinates.row][firstCoordinates.column]);
    Portal* secondPortal = dynamic_cast<Portal*>(
        m_tiles[secondCoordinates.row][secondCoordinates.column]);

    if (firstPortal != nullptr && secondPortal != nullptr) {
        firstPortal->setSiblingPortal(secondPortal);
        secondPortal->setSiblingPortal(firstPortal);
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

//getters
Tile* Level::getTile(int row, int col) {
    Tile* tileToReturn = m_tiles[row][col];
    return tileToReturn;
}

Tile* Level::getTile(Coordinates coordinates) {
    return getTile(coordinates.row, coordinates.column);
}

const std::vector<std::vector<Tile*> > Level::getTiles() const { return m_tiles; }
const std::vector<Character*> Level::getCharacters() const { return m_characters; }

Level::~Level() {}
