//
// Created by MBK on 12.11.25.
//
#ifndef PRAK_TILE_H
#define PRAK_TILE_H
#include "Character.h"
#include "PassiveAktive.h"
#include "EventBus.h"
#include <QtCore/qdebug.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
class Door;
class Tile
{
protected:
    char m_texture;
    Character *character{nullptr}; // Character on top of the tile, or nullptr if none.
    std::string m_texturePath;
    int m_row{};
    int m_column{};
    bool m_shouldMove{false}; // Should the player be moved to another tile (true if tile is portal)
    int m_djikstraExtraCost = 0; //
    inline static int AllowedDjikstraValueChanges = 0;
    Tile(int row, int col, char texture, std::string texturePath, int djikstraExtraCost=0);

public:
    virtual ~Tile();
    static Tile* GenerateTile(char texture, int row, int column, int djikstraExtraCost = 0);
    char getTexture() const;
    std::string getTexturePath() const;
    Character *getCharacter() const;
    bool hasCharacter() const;
    void setCharacter(Character *characterToPlace);
    int getRow() const;
    int getColumn() const;
    std::pair<int,int> getCordsAsPair() const;
    virtual bool onLeave(Tile *desTile);
    virtual std::pair<bool, Tile *> onEnter();
    void setTexturePath(std::string texturePath);
    virtual bool isEntrable() = 0;
    bool setDjikstraExtraCost(int cost); // returns success or failure
    int getDjikstraExtraCost() const;
    static void SetAllowedDjikstraValueChanges(int amount);

    // json
    // Tile* generateTileFromJSONState (TileState& tile );


};

class Floor : public Tile
{ // Accessible tile, characters can enter/leave them

public:
    Floor(int row, int column, int djikstraExtraCost=0)
        : Tile(row, column, '.', ":/pics/textures/floor/floor1.png", djikstraExtraCost) {};

    bool isEntrable() { return true; }
};

class Wall : public Tile
{ // Non-Accessible tile, characters can't enter them
public:
    Wall(int row, int column, int djikstraExtraCost=0)
        : Tile(row, column, '#', ":/pics/textures/wall/wall1.png", djikstraExtraCost) {};

    bool isEntrable() { return false; }
};

class Portal : public Tile, public EventListener
{
    bool shouldMove{true};
    Portal* m_siblingPortal = nullptr;
    int m_portalId;
    std::map<int, std::string> m_portalTexturesRegister = {{0, ":/pics/textures/portal/portal1.png"}, {1,":/pics/textures/portal/portal2.png"}, {2, ":/pics/textures/portal/portal3.png"}}; // portals have 3 different textures.

public:
    Portal(int row, int column, int portalId);

    bool isEntrable() override;

    void setPortal(Portal *portal);
    void setPortalId (int portalId);
    int getPortalId();
    Portal* getSiblingPortal();
    std::pair<bool, Tile *> onEnter() override;
    void onPortalCreation(PortalCreationEvent* event) override;
};

class basePlate : public Tile
{
    bool isOpen{false};

public:
    void switchStatus(bool status);
};

class Switch : public Tile, public Active
{

public:
    Switch(int row, int column, int djikstraExtraCost=0)
        : Tile(row, column, '?', ":/pics/textures/other tiles/switch.png", djikstraExtraCost) {}

    bool isEntrable() override { return true; }

    std::pair<bool, Tile *> onEnter() override;
};

class Door : public Tile, public Passive
{

    bool isOpen{false};
    bool state{0};    
    /* state 0 means wall, state 1 means floor*/
public:
    Door(int row, int column, bool isClosed=true, int djikstraExtraCost=0   )
        : Tile(row, column, 'X', ":/pics/textures/doors/door1.png", djikstraExtraCost) {
        if (!isClosed){
            Door::notify();
        }
    };
    void notify();
    bool isEntrable() { return state; }
};

class Pit : public Tile
{
public:
    Pit(int row, int column, int djikstraExtraCost=0)
        : Tile(row, column, '_', ":/pics/textures/other tiles/pit.png", djikstraExtraCost) {}

    bool isEntrable() override { return true; }

    bool onLeave(Tile *desTile) override;
};

class Ramp : public Tile
{
public:
    Ramp(int row, int column, int djikstraExtraCost=0)
        : Tile(row, column, '<' , ":/pics/textures/other tiles/ramp.png", djikstraExtraCost) {};

    bool isEntrable() { return true; }
};

class LevelChanger : public Tile
{
public:
    LevelChanger(int row, int column)
        : Tile(row, column, '$', ":/pics/textures/extra/levelchanger.png") {};

    bool isEntrable() { return true; }
};

class GameWinner : public Tile{
public:
    GameWinner(int row, int column)  : Tile(row, column, '!', ":/pics/textures/other tiles/winner.png"){};
    bool isEntrable() { return true; }

};

void to_json(json &jsonObject, const Tile* tileObject);
#endif //PRAK_TILE_H
