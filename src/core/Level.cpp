//
// Created by MBK on 13.11.25.
//

#include "Level.h"
#include "AttackController.h"
#include <TileManager.h>
#include <Utilities.h>
#include <qDebug>

Level::Level(int numberOfRows, int numberOfColumns)
    : m_numberOfRows(numberOfRows)
    , m_numberOfColumns(numberOfColumns) {
    m_tileManager = new TileManager(numberOfRows, numberOfColumns, this);
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
    Tile* replacedTile = m_tileManager->getTile(tileCoordinates);

    Tile* newTile = m_tileManager->createAndInsertOrReplaceTile(tileType, tileCoordinates);

    if (newTile->getTileType() == Types::TileType::LevelChanger) {
        emit levelFinished();
    }

    if (replacedTile != nullptr) {
        emit tileReplaced(replacedTile->getCoordinates(), newTile);
        delete replacedTile;
        replacedTile = nullptr;
    }
}

Character* Level::createAndInsertCharacter(Types::CharacterType characterType,
                                           Coordinates coordinates) {
    Character* character = Character::GenerateCharacter(characterType,
                                                        m_tileManager->getTile(coordinates));

    character->setTile(m_tileManager->getTile(coordinates));

    m_characters.push_back(character);

    return character;
}

void Level::connectPortals(Coordinates firstCoordinates, Coordinates secondCoordinates) {
    Portal* firstPortal = dynamic_cast<Portal*>(m_tileManager->getTile(firstCoordinates));

    Portal* secondPortal = dynamic_cast<Portal*>(m_tileManager->getTile(secondCoordinates));

    if (firstPortal != nullptr && secondPortal != nullptr) {
        firstPortal->setSiblingPortal(secondPortal);
        secondPortal->setSiblingPortal(firstPortal);
    }
}

void Level::initializeAllTilesToFloor() {
    m_tileManager->initializeAllTilesToType(Types::TileType::Floor);
}

Level::LevelValidationResult Level::validateSelf() const {
    switch (getHumansCount()) {
    case 0:
        return LevelValidationResult::HumanCharacterMissing;
    case 1:
        break;
    default:
        return LevelValidationResult::MoreThanOneHuman;
    }

    if (m_characters.size() <= 1) {
        return LevelValidationResult::ComputerCharacterMissing;
    }

    if (arePortalsConnected() == false) {
        return LevelValidationResult::PortalConnectionsMissing;
    }
    return LevelValidationResult::Success;
}

void Level::clear() { m_tileManager->clearTiles(); }

//getters
int Level::getNumberOfRows() const { return m_numberOfRows; }

int Level::getNumberOfColumns() const { return m_numberOfColumns; }

int Level::getHumansCount() const {
    int humansCounter = 0;
    for (Character* character : m_characters) {
        if (character->getCharacterType() == Types::CharacterType::Human) {
            humansCounter++;
        }
    }
    return humansCounter;
}

bool Level::arePortalsConnected() const {
    for (std::vector<Tile*> row : m_tileManager->getTiles()) {
        for (Tile* tile : row) {
            if (tile->getTileType() == Types::TileType::Portal) {
                Portal* converted = dynamic_cast<Portal*>(tile);

                if (converted
                    && (converted->getSiblingPortal() == nullptr
                        || converted->getSiblingPortal()->getSiblingPortal() != converted)) {
                    return false;
                }
            }
        }
    }
    return true;
}

Tile* Level::getTile(int row, int col) { return getTile({row, col}); }

Tile* Level::getTile(Coordinates coordinates) { return m_tileManager->getTile(coordinates); }

const std::vector<std::vector<Tile*> > Level::getTiles() const { return m_tileManager->getTiles(); }
const std::vector<Character*> Level::getCharacters() const { return m_characters; }

Level::~Level() { delete m_tileManager; }
