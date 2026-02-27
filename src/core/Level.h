//
// Created by MBK on 13.11.25.
//

#ifndef PRAK_LEVEL_H
#define PRAK_LEVEL_H
#include "tile.h"
#include "LevelGraph.h"
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
    inline static std::vector<std::vector<Tile*>> TILES = {};
    std::vector<Character *> m_characters;
    std::vector<Character *> nonPlayableCharacters;
    std::pair <int,int> m_playingCharacterPosition;
    int m_gameHeight = 10;
    int m_gameWidth = 10;
    LevelGraph* m_graph = new LevelGraph();
    void addCharacter(Character* character);

public: //static functions
    static Tile* GetTile(int row, int col);
    static Tile* GetTile(Coordinates coordinates);

public:
    Level(std::string gameBoard);
    const std::vector<std::vector<Tile*>> getTiles() const;
    int getHeight() const;
    int getWidth() const;
    void setDefaultTiles();
    ~Level();

};

#endif //PRAK_LEVEL_H
