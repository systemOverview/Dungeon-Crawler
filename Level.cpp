//
// Created by MBK on 13.11.25.
//

#include "Level.h"
#include "GuardController.h"
#include "StationaryController.h"
#include "AttackController.h"
#include <qDebug>

Level::Level(int height, int width, std::string gameString)
{
    gameHeight = height;
    gameWidth = width;
    setDefaultTiles();
    std::map<int, Tile*> charactersTiles;
    std::vector<Door *> doors;
    Switch *theSwitch = nullptr;
    std::map<int,std::vector<std::pair<int,int>>> portalsCords; //portalId : vector containing cords of portals with that id

    for (int i=0; i<gameString.length(); i++){
        int row = i / 10;
        int column = i % 10;
        if (gameString[i]=='0' || gameString[i]=='1' || gameString[i]=='2'){
            int portalIdConvertedToInt = gameString[i] - '0';
            portalsCords[portalIdConvertedToInt].push_back({row, column});

        }
        Tile* tile = Tile::GenerateTile(gameString[i], row, column);
        (tiles)[row][column] = tile;
        if (dynamic_cast<Door*>(tile)!=nullptr){
            doors.push_back(dynamic_cast<Door*>(tile));
        }
        else if (dynamic_cast<Switch*>(tile)!=nullptr){
            theSwitch = dynamic_cast<Switch*>(tile);
        }
        if (gameString[i]!='#'){m_graph->addVertex(tile, 1);} // wall, don't add to graph
        if (gameString[i]=='P' || gameString[i]=='S' || gameString[i] == 'G' || gameString[i]=='A'){charactersTiles.insert({i, tile});} // character.

    }

    for (auto const& [stringIndex,tile] : charactersTiles){
        if (gameString[stringIndex]=='P'){m_playingCharacterPosition = {stringIndex/10, stringIndex%10}; continue;}
        Character* character = Character::GenerateCharacter(gameString[stringIndex], tile, this, m_graph);
        placeCharacter(character,stringIndex/10 , stringIndex%10, false);


    }

    setPortals(portalsCords);
    for (auto door : doors) {
        theSwitch->attach(door);
    }
    m_graph->setupAlldges();


}

Tile *Level::getTile(int row, int col)
{
    Tile *tileToReturn = tiles[row][col];
    return tileToReturn;
}

std::vector<std::vector<Tile *>> *Level::getTiles()
{
    return &tiles;
}

int Level::getHeight() const
{
    return gameHeight;
}

int Level::getWidth() const
{
    return gameWidth;
}

void Level::placeCharacter(Character *c, int row, int col, bool isPlayable)
{
    Tile *tileToPlaceAt = tiles[row][col];
    tileToPlaceAt->setCharacter(c);
    c->setTile(tileToPlaceAt);
    characters.push_back(c);
    if (isPlayable) {
        playableCharacter = c;
    } else {
        nonPlayableCharacters.push_back(c);
    }
}

void Level::placePlayingCharacter(Character *c)
{
    // this function is called by the dungeon controller, since the playing character is the same across all levels
    // everytime a player levels up, the controller calls this to place the character. The other characters (NPCs)
    // don't move across levels.

    placeCharacter(c, m_playingCharacterPosition.first, m_playingCharacterPosition.second, true);
}

Character *Level::getPlayableCharacter()
{
    return playableCharacter;
}

std::vector<Character *> Level::getNonPlayableCharacters()
{
    return nonPlayableCharacters;
}

void Level::setPortals(std::map<int, std::vector<std::pair<int,int>>> portalsCords)
{

    for (int i=0; i<3; i++){
        std::pair<int,int> firstCords =portalsCords[i].at(0);
        std::pair<int,int> secondCords =portalsCords[i].at(1);

        Portal* firstPortal = new Portal(firstCords.first, firstCords.second, i);
        Portal* secondPortal = new Portal(secondCords.first, secondCords.second, i);
        (tiles)[firstCords.first][firstCords.second] = firstPortal;
        (tiles)[secondCords.first][secondCords.second] = secondPortal;
        firstPortal->setPortal(secondPortal);
        secondPortal->setPortal(firstPortal);
        m_graph->addVertex(firstPortal, 1);
        m_graph->addVertex(secondPortal, 1);

    }
}

void Level::setDefaultTiles()
{
    for (int i = 0; i < gameHeight; i++) {
        std::vector<Tile *> row;
        tiles.push_back(row);
        for (int j = 0; j < gameWidth; j++) {
            tiles[i].push_back(nullptr);
        }
    }
}

LevelGraph *Level::getGraph()
{
    return m_graph;
}
