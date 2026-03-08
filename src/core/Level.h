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
private:
    std::vector<Character*> m_characters = {};
    int m_numberOfRows = 10;
    int m_numberOfColumns = 10;

    TileManager* m_tileManager = nullptr;

    int getHumansCount() const;
    bool arePortalsConnected() const;

public:
    enum class LevelValidationResult {
        Success,
        PortalConnectionsMissing,
        HumanCharacterMissing,
        MoreThanOneHuman,
        ComputerCharacterMissing
    };
    Tile* getTile(int row, int col);
    Tile* getTile(Coordinates coordinates);
    Character* whoIsOccupyingTile(Tile* tile);
signals:
    void tileReplaced(Coordinates coordinatesOfReplacedTile, Tile* newTile);

public:
    Level(int numberOfRows = GameSettings::TILES_PER_SIDE,
          int numberOfColumns = GameSettings::TILES_PER_SIDE);

    Level(json levelJson,
          int numberOfRows = GameSettings::TILES_PER_SIDE,
          int numberOfColumns = GameSettings::TILES_PER_SIDE);

    const std::vector<std::vector<Tile*>> getTiles() const;
    const std::vector<Character*> getCharacters() const;

    void createAndInsertOrReplaceTile(Types::TileType tileType, Coordinates tileCoordinates);
    Character* createAndInsertCharacter(Types::CharacterType characterType, Coordinates coordinate);

    void connectPortals(Coordinates firstCoordinates, Coordinates secondCoordinates);
    void initializeAllTilesToFloor();

    LevelValidationResult validateSelf() const;

    void clear();
    ~Level();
    int getNumberOfRows() const;
    int getNumberOfColumns() const;
};

#endif //PRAK_LEVEL_H
