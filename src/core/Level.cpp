//
// Created by MBK on 13.11.25.
//

#include "Level.h"
#include "AttackController.h"
#include "DungeonCrawler.h"
#include "StationaryController.h"
#include <qDebug>

void Level::addCharacter(Character* character)
{
    m_characters.push_back(character);
    if (character->getTexture()!='P') {
        nonPlayableCharacters.push_back(character);
    }
}

Level::Level(std::string gameString) {
    setDefaultTiles();
    for (int i = 0; i < gameString.length(); i++) {
        int row = i / 10;
        int column = i % 10;
        Tile* tile = Tile::GenerateTile(gameString[i], row, column);
        (TILES)[row][column] = tile;

        if (gameString[i] == 'P' || gameString[i] == 'S' || gameString[i] == 'G'
            || gameString[i] == 'A') {
            Character* character = Character::GenerateCharacter(gameString[i], tile, this);
            tile->setCharacter(character);
        }
    }
}

Tile* Level::GetTile(int row, int col) {
    Tile* tileToReturn = TILES[row][col];
    return tileToReturn;
}

Tile* Level::GetTile(Coordinates coordinates) {
    return GetTile(coordinates.row, coordinates.column);
}

const std::vector<std::vector<Tile*> > Level::getTiles() const { return TILES; }

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
        TILES.push_back(row);
        for (int j = 0; j < m_gameWidth; j++) {
            TILES[i].push_back(nullptr);
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
    for (std::vector<Tile*> row : TILES) {
        for (Tile* tile : row){
            delete tile;
            tile = nullptr;
        }
        row.clear();
    }
    TILES.clear();
    for (Character* NPC : nonPlayableCharacters){
        delete NPC;
        NPC = nullptr;
    }
    nonPlayableCharacters.clear();
    delete m_graph;
}
