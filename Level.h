//
// Created by MBK on 13.11.25.
//

#ifndef PRAK_LEVEL_H
#define PRAK_LEVEL_H
#include "tile.h"
#include <iostream>
#include "LevelGraph.h"
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
class Character;
class Tile;
class Level
{
    inline static Character* HumanCharacter = Character::GenerateCharacter('P');
    Tile* m_initialHumanCharacterTile = nullptr;
    // Because the human player doesn't get deleted when level changes (to keep health data)
    // humanCharacter is static, and when levels are initially constructed from a string, they save the tile where the characater is supposed to be
    // and only when the level is activeted through activate() does the level place the character at that tile
    std::vector<std::vector<Tile *>> tiles;
    std::vector<Character *> m_characters;
    std::vector<Character *> nonPlayableCharacters;
    std::pair <int,int> m_playingCharacterPosition;
    int m_gameHeight = 10;
    int m_gameWidth = 10;
    LevelGraph* m_graph = new LevelGraph();
    void addCharacter(Character* character);
    bool m_isActivated = false;


public:
    Level(int height, int width, std::string gameBoard, bool isActive = false);
    Level(json levelJson);
    Tile *getTile(int row, int col);
    std::vector<std::vector<Tile *>> *getTiles();
    int getHeight() const;
    int getWidth() const;
    Character *getPlayableCharacter();
    std::vector<Character *> getNonPlayableCharacters();
    void setDefaultTiles();
    LevelGraph* getGraph();
    void activateLevel();
    Tile* getInitialHumanCharacterTile() const;
    bool isActivated() const;
    void removeCharacter(Character* vharacter);
    ~Level();

};

#endif //PRAK_LEVEL_H
