#include "JsonObjects.h"
#include "Level.h"
#include "Utilities.h"
#include "fstream"

json JsonGenerator::TileToJson(Tile* tile) {
    std::string tileType = Utilities::Q_ENUM_ToQString(tile->getTileType()).toStdString();
    json tileJSON = json::object(
        {{"type", tileType}, {"row", tile->getRow()}, {"column", tile->getColumn()}});
    return tileJSON;
}

void JsonGenerator::SaveLevelToJson(Level* level, QString path) {
    std::ofstream file(path.toStdString());
    json root;

    auto tilesArray = json::array();

    for (std::vector<Tile*> row : level->getTiles()) {
        for (Tile* tile : row) {
            tilesArray.push_back(TileToJson(tile));
        }
    }

    root["tiles"] = tilesArray;
    file << std::setw(4) << root;
}
