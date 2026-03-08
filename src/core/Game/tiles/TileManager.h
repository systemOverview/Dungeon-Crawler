#ifndef TILEMANAGER_H
#define TILEMANAGER_H

#include <QObject>
#include "Constants.h"
class Tile;
class TileManager : public QObject
// This was made into a class to centralize and manage any connections between tiles ( switch -> door activation ).
{
    Q_OBJECT
private:
    int m_numberOfRows{};
    int m_numberOfColumns{};

    std::vector<std::vector<Tile*>> m_tiles;
    Tile* generateTile(Types::TileType tileType, int row, int column);
    Tile* generateTile(Types::TileType tileType, Coordinates coordinates);

signals:
    void switchTriggered();

public:
    TileManager(int numberOfRows, int numberOfColumns, QObject* parent = nullptr);

    Tile* getTile(Coordinates tileCoordinates) const;
    Tile* createAndInsertOrReplaceTile(Types::TileType tileType, Coordinates coordinates);

    std::vector<std::vector<Tile*>> getTiles() const;
    void initializeAllTilesToType(Types::TileType type);

    void clearTiles();

signals:
};

#endif // TILEMANAGER_H
