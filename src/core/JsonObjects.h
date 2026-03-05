#ifndef JSONOBJECTS_H
#define JSONOBJECTS_H

class Level;
class Tile;
class QString;
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class JsonGenerator
{
private:
    static json TileToJson(Tile* tile);

public:
    static void SaveLevelToJson(Level* level, QString path);
};

#endif
