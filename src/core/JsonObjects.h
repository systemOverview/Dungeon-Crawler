#ifndef JSONOBJECTS_H
#define JSONOBJECTS_H
#include "Constants.h"
#include <nlohmann/json.hpp>

class Level;
class Tile;
class Portal;
class Character;
class QString;

class QJsonObject;
using json = nlohmann::json;

class JsonGenerator
{
private:
    static QJsonObject TileToJson(Tile* tile);
    static void JsonToTile(Level* level, QJsonObject tileJSON);

    static QJsonObject CharacterToJson(Character* character);
    static void JsonToCharacter(Level* level, QJsonObject characterJSON);

    static QJsonObject PortalConnectionToJson(Portal* portal);
    static void JsonToPortalConnection(Level* level, QJsonObject portalConnectionJSON);

public:
    static void SaveLevelToJson(Level* level, QString path);
    static Level* CreateLevelFromJsonFile(QString filePath);
};

#endif
