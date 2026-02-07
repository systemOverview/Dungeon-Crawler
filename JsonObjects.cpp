#include <nlohmann/json.hpp>
#include "Constants.h"
#include "JsonObjects.h"
#include "tile.h"
#include <fstream>

using JSON = nlohmann::json;

void JsonGenerator::generateTiles()
{
    {
        std::ofstream myfile("/Users/mbk/prak/gamedata/level.json");
        auto tilesArray = json::array();

        std::string gameString = GameData::GameStrings[1];
        for (int i=0; i<gameString.length(); i++){
            int row = i / 10;
            int column = i % 10;
            JSON jsonTile = {
                {"texture",  gameString[i]},
                {"row", row},
                {"column", column}
            };
            tilesArray.push_back(jsonTile);

        }
        JSON tileHolder = {{"Tiles", tilesArray}};
        myfile << std::setw(4) << tileHolder << std::endl;
    }
}

void JsonGenerator::saveGameState(std::vector<Level *> levels)
{
    std::ofstream myfile(GameData::DataFilePath);
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
    myfile << std::setw(4) << root;




}
