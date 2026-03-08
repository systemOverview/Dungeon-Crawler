#include "JsonObjects.h"
#include <QtCore/qjsonarray.h>
#include <QtCore/qjsonobject.h>
#include "Level.h"
#include "Utilities.h"
#include "fstream"
#include <qfile.h>
#include <qjsondocument.h>

QJsonObject JsonGenerator::TileToJson(Tile* tile) {
    QString tileType = Utilities::Q_ENUM_ToQString(tile->getTileType());

    QJsonObject tileJSON;

    tileJSON.insert("type", tileType);
    tileJSON.insert("row", tile->getRow());
    tileJSON.insert("column", tile->getColumn());

    return tileJSON;
}

void JsonGenerator::JsonToTile(Level* level, QJsonObject tileJSON) {
    Types::TileType tileType = Utilities::QString_To_Q_ENUM<Types::TileType>(
        (tileJSON["type"]).toString());

    int row = tileJSON["row"].toInt();
    int column = tileJSON["column"].toInt();

    level->createAndInsertOrReplaceTile(tileType, {row, column});
}
QJsonObject JsonGenerator::CharacterToJson(Character* character) {
    QString characterType = Utilities::Q_ENUM_ToQString(character->getCharacterType());

    QJsonObject characterJSON;

    characterJSON.insert("type", characterType);
    characterJSON.insert("row", character->getTile()->getRow());
    characterJSON.insert("column", character->getTile()->getColumn());
    characterJSON.insert("health", character->getCurrentHealthPoints());

    return characterJSON;
}

void JsonGenerator::JsonToCharacter(Level* level, QJsonObject characterJSON) {
    Types::CharacterType characterType = Utilities::QString_To_Q_ENUM<Types::CharacterType>(
        (characterJSON["type"]).toString());

    int row = characterJSON["row"].toInt();
    int column = characterJSON["column"].toInt();

    level->createAndInsertCharacter(characterType, {row, column});
}

QJsonObject JsonGenerator::PortalConnectionToJson(Portal* portal) {
    Portal* siblingPortal = portal->getSiblingPortal();
    QJsonObject portalJSON;

    portalJSON.insert("firstPortalRow", portal->getRow());
    portalJSON.insert("firstPortalColumn", portal->getColumn());
    portalJSON.insert("secondPortalRow", siblingPortal->getRow());
    portalJSON.insert("secondPortalColumn", siblingPortal->getColumn());

    return portalJSON;
}

void JsonGenerator::JsonToPortalConnection(Level* level, QJsonObject portalConnectionJSON) {
    int firstPortalRow = portalConnectionJSON["firstPortalRow"].toInt();
    int firstPortalColumn = portalConnectionJSON["firstPortalColumn"].toInt();
    int secondPortalRow = portalConnectionJSON["secondPortalRow"].toInt();
    int secondPortalColumn = portalConnectionJSON["firstPortalColumn"].toInt();

    level->connectPortals({firstPortalRow, firstPortalColumn},
                          {secondPortalRow, secondPortalColumn});
}

void JsonGenerator::SaveLevelToJson(Level* level, QString path) {
    QFile file(path);
    file.open(QFile::WriteOnly);

    QJsonDocument document;

    QJsonObject rootObject;

    QJsonArray tilesArray;
    QJsonArray charactersArray;
    QJsonArray portalConnectionsArray;
    QJsonObject levelInfo;

    std::vector<Portal*> convertedPortals;

    for (const std::vector<Tile*>& row : level->getTiles()) {
        for (Tile* tile : row) {
            tilesArray.push_back(TileToJson(tile));

            if (tile->getTileType() == Types::TileType::Portal) {
                Portal* portal = dynamic_cast<Portal*>(tile);

                if (std::find(convertedPortals.begin(),
                              convertedPortals.end(),
                              portal->getSiblingPortal())
                    == convertedPortals.end()) {
                    portalConnectionsArray.push_back(PortalConnectionToJson(portal));
                    convertedPortals.push_back(portal);
                }
            }
        }
    }

    for (Character* character : level->getCharacters()) {
        if (character != nullptr) {
            charactersArray.push_back(CharacterToJson(character));
        }
    }

    levelInfo.insert("numberOfRows", level->getNumberOfRows());
    levelInfo.insert("numberOfColumns", level->getNumberOfColumns());

    rootObject.insert("tiles", tilesArray);
    rootObject.insert("characters", charactersArray);
    rootObject.insert("portalConnections", portalConnectionsArray);
    rootObject.insert("levelInfo", levelInfo);

    document.setObject(rootObject);
    file.write(document.toJson());
}

Level* JsonGenerator::CreateLevelFromJsonFile(QString filePath) {
    QFile file(filePath);
    file.open(QFile::ReadOnly);
    QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll());

    QJsonObject levelInfo = jsonDoc.object().find("levelInfo")->toObject();
    Level* level = new Level(levelInfo.find("numberOfRows")->toInt(),
                             levelInfo.find("numberOfColumns")->toInt());

    QJsonArray tilesJSON = jsonDoc.object().find("tiles")->toArray();
    QJsonArray charactersJSON = jsonDoc.object().find("characters")->toArray();
    QJsonArray portalsConnnectionsJSON = jsonDoc.object().find("portalConnections")->toArray();

    for (QJsonValueRef tileJsonRef : tilesJSON) {
        JsonToTile(level, tileJsonRef.toObject());
    }
    for (QJsonValueRef tileJsonRef : charactersJSON) {
        JsonToCharacter(level, tileJsonRef.toObject());
    }
    for (QJsonValueRef tileJsonRef : portalsConnnectionsJSON) {
        JsonToPortalConnection(level, tileJsonRef.toObject());
    }

    return level;
}

