#include "TileManager.h"
#include "tile.h"


TileManager::TileManager(int numberOfRows, int numberOfColumns, QObject* parent)
    : QObject(parent)
    , m_numberOfRows(numberOfRows)
    , m_numberOfColumns(numberOfColumns) {
    m_tiles = std::vector<std::vector<Tile*>>(numberOfRows, std::vector<Tile*>(numberOfColumns));
}

Tile* TileManager::getTile(Coordinates tileCoordinates) const {
    return m_tiles[tileCoordinates.row][tileCoordinates.column];
}

std::vector<std::vector<Tile*>> TileManager::getTiles() const { return m_tiles; }

void TileManager::initializeAllTilesToType(Types::TileType type) {
    // this does not emit a tileReplaced signal, only use before any tiles are inserted.
    for (int i = 0; i < m_numberOfRows; i++) {
        for (int j = 0; j < m_numberOfRows; j++) {
            createAndInsertOrReplaceTile(type, {i, j});
        }
    }
}

Tile* TileManager::generateTile(Types::TileType tileType, int row, int column) {
    switch (tileType) {
    case Types::TileType::Floor:
        return new Floor(row, column);

    case Types::TileType::Wall:
        return new Wall(row, column);

    case Types::TileType::OpenDoor: {
        {
            Door* door = new Door(Types::TileType::OpenDoor, row, column);
            connect(this, &TileManager::switchTriggered, door, &Door::switchDoorStatus);
            return door;
        }
    }
    case Types::TileType::ClosedDoor: {
        Door* door = new Door(Types::TileType::ClosedDoor, row, column);
        connect(this, &TileManager::switchTriggered, door, &Door::switchDoorStatus);
        return door;
    }
    case Types::TileType::Switch: {
        Switch* switchTile = new Switch(row, column);
        connect(switchTile,
                &Switch::tileAccessed,
                this,
                &TileManager::switchTriggered); // signal to signal
        return switchTile;
    }
    case Types::TileType::Pit:

        return new Pit(row, column);
    case Types::TileType::Ramp:
        return new Ramp(row, column);

    case Types::TileType::LevelChanger:
        return new LevelChanger(row, column);
    case Types::TileType::GameWinner:
        return new GameWinner(row, column);
    case Types::TileType::Portal:
        return new Portal(row, column);

    default:
        return new Floor(row, column);
    }
}

Tile* TileManager::generateTile(Types::TileType tileType, Coordinates coordinates) {
    return generateTile(tileType, coordinates.row, coordinates.column);
}

Tile* TileManager::createAndInsertOrReplaceTile(Types::TileType tileType, Coordinates coordinates) {
    Tile* newTile = generateTile(tileType, coordinates);
    m_tiles[coordinates.row][coordinates.column] = newTile;
    return newTile;
}

void TileManager::clearTiles() {
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
