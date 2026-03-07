#ifndef JSONOBJECTS_H
#define JSONOBJECTS_H
#include <nlohmann/json.hpp>

class Level;
class Tile;
class Portal;
class Character;
class QString;

using json = nlohmann::json;

class JsonGenerator
{
private:
    static json TileToJson(Tile* tile);

    static json CharacterToJson(Character* character);
    static json PortalConnectionToJson(Portal* portal);

public:
    static void SaveLevelToJson(Level* level, QString path);
    static json parseFileJson(QString filePath);
};

#endif
