//
// Created by MBK on 13.11.25.
//

#include "Level.h"
#include "GuardController.h"
#include "StationaryController.h"
#include "AttackController.h"
#include <qDebug>

void Level::addCharacter(Character* character)
{
    m_characters.push_back(character);
    if (character->getTexture()!='P') {
        nonPlayableCharacters.push_back(character);
    }
}

Level::Level(int height, int width, std::string gameString, bool isActive) :
    m_gameHeight {height}, m_gameWidth{width}
{

    setDefaultTiles();
    std::map<int, Tile*> charactersTiles;
    std::vector<Door *> doors;
    Switch *theSwitch = nullptr;
    for (int i=0; i<gameString.length(); i++){
        int row = i / 10;
        int column = i % 10;
        Tile* tile = Tile::GenerateTile(gameString[i], row, column);
        (tiles)[row][column] = tile;
        if (dynamic_cast<Door*>(tile)!=nullptr){
            doors.push_back(dynamic_cast<Door*>(tile));
        }
        else if (dynamic_cast<Switch*>(tile)!=nullptr){
            theSwitch = dynamic_cast<Switch*>(tile);
        }
        if (gameString[i]!='#'){m_graph->addVertex(tile, 1);} // wall, don't add to graph
        if (gameString[i]=='P' || gameString[i]=='S' || gameString[i] == 'G' || gameString[i]=='A'){
            if (gameString[i]=='P'){
                m_initialHumanCharacterTile = tile;
                addCharacter(HumanCharacter);
                continue;
            }
            qDebug() << gameString[i] << tile->getCordsAsPair();
            Character* character = Character::GenerateCharacter(gameString[i], -1, tile, this, m_graph);
            addCharacter(character);
    }

    if (theSwitch){
        for (auto door : doors) {
            theSwitch->attach(door);
        }
    }
    }
    m_graph->setupAlldges();
    assert(m_initialHumanCharacterTile!=nullptr && "No tile for the human character was assigned" );
}

Level::Level(json levelJson)
{
    setDefaultTiles();
    std::map<int, Tile*> charactersTiles;
    std::vector<Door *> doors;
    Switch *theSwitch = nullptr;
    std::cout << levelJson["Level"]["Characters"];

    for (const auto& tileJson : levelJson["Level"]["Tiles"]){
        // std::cout << tileJson.at("row").get<int>() ;
        char texture = tileJson.at("texture").get<char>();
        Tile* tile = Tile::GenerateTile(texture, tileJson.at("row"), tileJson.at("column"), tileJson.at("Djikstra Extra Cost"));
        (tiles)[tileJson.at("row")][tileJson.at("column")] = tile;

        if (dynamic_cast<Door*>(tile)!=nullptr){
            doors.push_back(dynamic_cast<Door*>(tile));
        }
        else if (dynamic_cast<Switch*>(tile)!=nullptr){
            theSwitch = dynamic_cast<Switch*>(tile);
        }
        if (texture!='#'){m_graph->addVertex(tile, 1);} // wall, don't add to graph

    }
    for (const auto& characterJson : levelJson["Level"]["Characters"]){

        char texture = characterJson.at("texture").get<char>();
        int row = characterJson.at("row").get<int>();
        int col = characterJson.at("column").get<int>();
        int HP = characterJson.at("HP").get<int>();
        std::cout <<"char"<<  texture;
        Tile* tile = tiles[row][col];
            if (texture=='P'){
                m_initialHumanCharacterTile = tile;
                if (HP!=-100){ // sentinel value, explained in jsonobjects.cpp
                HumanCharacter->decrementFromHP(HumanCharacter->getMaxHP() - HP);
                }
                addCharacter(HumanCharacter);
                continue;
            }
            qDebug() << texture << tile->getCordsAsPair();
            Character* character = Character::GenerateCharacter(texture, HP, tile, this, m_graph);
            addCharacter(character);
    }

    if (theSwitch){
        for (auto door : doors) theSwitch->attach(door);
    }

    m_graph->setupAlldges();
    assert(m_initialHumanCharacterTile!=nullptr && "No tile for the human character was assigned" );
    qDebug() << "generated";
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
    return m_gameHeight;
}

int Level::getWidth() const
{
    return m_gameWidth;
}

Character *Level::getPlayableCharacter()
{
    return HumanCharacter;
}

std::vector<Character *> Level::getNonPlayableCharacters()
{
    return nonPlayableCharacters;
}


void Level::setDefaultTiles()
{
    for (int i = 0; i < m_gameHeight; i++) {
        std::vector<Tile *> row;
        tiles.push_back(row);
        for (int j = 0; j < m_gameWidth; j++) {
            tiles[i].push_back(nullptr);
        }
    }
}

LevelGraph *Level::getGraph()
{
    return m_graph;
}

void Level::activateLevel()
{
    for (auto character : m_characters){
        Tile* tileToPlaceAt = character->getTile();

        if (character->getTexture()=='P'){
            tileToPlaceAt = m_initialHumanCharacterTile;
        }
        assert(tileToPlaceAt!=nullptr);
        tileToPlaceAt->setCharacter(character);
        character->setTile(tileToPlaceAt);

    }
    m_isActivated = true;
    Tile::SetAllowedDjikstraValueChanges(3);
}

Tile *Level::getInitialHumanCharacterTile() const
{
    return m_initialHumanCharacterTile;
}

bool Level::isActivated() const
{
    return m_isActivated;
}

void Level::removeCharacter(Character *character)
{
    for (auto it = m_characters.begin(); it!=m_characters.end();)
    {
        if (*it==character){
            it = m_characters.erase(it);
        }
        else{
            it++;
        }
    }

    for (auto it = nonPlayableCharacters.begin(); it!=nonPlayableCharacters.end();)
    {
        if (*it==character){
            it = nonPlayableCharacters.erase(it);
        }
        else{
            it++;
        }
    }
}

Level::~Level()
{
    for (std::vector<Tile*> row : tiles){
        for (Tile* tile : row){
            delete tile;
            tile = nullptr;
        }
        row.clear();
    }
    tiles.clear();
    for (Character* NPC : nonPlayableCharacters){
        delete NPC;
        NPC = nullptr;
    }
    nonPlayableCharacters.clear();
    delete m_graph;
}
