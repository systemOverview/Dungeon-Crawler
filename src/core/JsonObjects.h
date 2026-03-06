#ifndef JSONOBJECTS_H
#define JSONOBJECTS_H
#include <nlohmann/json.hpp>

class Level;
class Tile;
class QString;
class Character;

using json = nlohmann::json;

class JsonGenerator
{
private:
    static json TileToJson(Tile* tile);

    static json CharacterToJson(Character* character);

public:
    static void SaveLevelToJson(Level* level, QString path);
    static json parseFileJson(QString filePath);
};

#endif
