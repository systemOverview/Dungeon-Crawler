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
json JsonGenerator::CharacterToJson(Character* character) {
    std::string characterType = Utilities::Q_ENUM_ToQString(character->getCharacterType())
                                    .toStdString();
    json characterJSON = json::object({{"type", characterType},
                                       {"row", character->getTile()->getRow()},
                                       {"column", character->getTile()->getColumn()},
                                       {"health", character->getCurrentHealthPoints()}});
    return characterJSON;
}

void JsonGenerator::SaveLevelToJson(Level* level, QString path) {
    std::ofstream file(path.toStdString());

    json root;

    auto tilesArray = json::array();
    auto charactersArray = json::array();

    for (const std::vector<Tile*>& row : level->getTiles()) {
        for (Tile* tile : row) {
            tilesArray.push_back(TileToJson(tile));
        }
    }

    for (Character* character : level->getCharacters()) {
        if (character != nullptr) {
            charactersArray.push_back(CharacterToJson(character));
        }
    }

    root["tiles"] = tilesArray;
    root["characters"] = charactersArray;

    file << std::setw(4) << root;
}

json JsonGenerator::parseFileJson(QString filePath) {
    std::ifstream file(filePath.toStdString());
    json parsedJson = json::parse(file);
    return parsedJson;
}
