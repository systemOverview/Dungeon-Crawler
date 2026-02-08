#include "JsonObjects.h"
#include <QStandardPaths>
#include <QtCore/qdir.h>
#include "Constants.h"
#include "tile.h"
#include <fstream>
#include <nlohmann/json.hpp>


void JsonGenerator::saveGameState(std::vector<Level *> levels)
{
    QString storagePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    std::string filePath = storagePath.toStdString() + "/level.json";
    QDir qdir;
    if (!qdir.mkpath(storagePath)) {
        qDebug() << "failed to create dir";
    }
    std::ofstream file(filePath);

    json root;
    auto levelsArray = json::array();

    for (Level* level : levels) {

        json levelHolder;
        auto tilesArray = json::array();
        auto charactersArray = json::array();
        if (!level->isActivated()){
            Tile* humanTile = level->getInitialHumanCharacterTile();
            assert(humanTile!=nullptr && "Initial human tile is not set.");
            charactersArray.push_back({
                                       {"HP", -100},
                                       //-100 tells level() not to update the
                                       // human health, because the only reason
                                       // a level isnt active is because it isnt reached yet,
                                       // which means another level has already created the static
                                       // HumanCharacter and set its health
                                       {"column", humanTile->getColumn()},
                                       {"row", humanTile->getRow()},
                                       {"texture", 'P'}
                                       });
        }

        for (auto& tileRow : *level->getTiles()) {
            for (Tile* tile : tileRow) {
                tilesArray.push_back(tile);
                if (tile->getCharacter())
                    charactersArray.push_back(tile->getCharacter());
            }
        }

        levelHolder["Tiles"] = tilesArray;
        levelHolder["Characters"] = charactersArray;

        json levelJson;
        levelJson["Level"] = levelHolder;
        levelsArray.push_back(levelJson);
    }

    root["Levels"] = levelsArray;
    file << std::setw(4) << root;
}
