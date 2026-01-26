//
// Created by MBK on 12.11.25.
//

#ifndef PRAK_TILE_H
#define PRAK_TILE_H
#include "Character.h"
#include "PassiveAktive.h"
#include "Subject.h"
#include "EventBus.h"
#include <QtCore/qdebug.h>
class Door;
class Tile : public Subject
{
protected:
    char m_texture;
    Character *character{nullptr}; // Character on top of the tile, or nullptr if none.
    std::string m_texturePath;
    int m_row{};
    int m_column{};
    bool m_shouldMove{false}; // Should the player be moved to another tile (true if tile is portal)

    Tile(int row, int col, char texture, std::string texturePath);

public:
    virtual ~Tile();
    static Tile* GenerateTile(char texture, int row, int column);
    char getTexture() const;
    std::string getTexturePath() const;
    Character *getCharacter() const;
    bool hasCharacter() const;
    void setCharacter(Character *characterToPlace);
    int getRow() const;
    int getColumn() const;
    std::pair<int,int> getCordsAsPair() const;
    virtual bool moveTo(Tile *desTile, Character *who);
    virtual bool onLeave(Tile *desTile, Character *who);
    virtual std::pair<bool, Tile *> onEnter(Character *who);
    void setTexturePath(std::string texturePath);
    virtual bool isEntrable() = 0;
};

class Floor : public Tile
{ // Accessible tile, characters can enter/leave them


public:
    Floor(int row, int column)
        : Tile(row, column, '.', ":/pics/textures/floor/floor1.png") {};

    bool isEntrable() { return true; }
};

class Wall : public Tile
{ // Non-Accessible tile, characters can't enter them

public:
    Wall(int row, int column)
        : Tile(row, column, '#', ":/pics/textures/wall/wall1.png") {};

    bool isEntrable() { return false; }
};

class Portal : public Tile
{
    bool shouldMove{true};
    Portal *portalToAccess;
    int m_portalId;
    std::map<int, std::string> m_portalTexturesRegister = {{0, ":/pics/textures/portal/portal1.png"}, {1,":/pics/textures/portal/portal2.png"}, {2, ":/pics/textures/portal/portal3.png"}}; // portals have 3 different textures.

public:
    Portal(int row, int column, int portalId);;

    bool isEntrable() override;

    void setPortal(Portal *portal);
    void setPortalId (int portalId);

    std::pair<bool, Tile *> onEnter(Character *who) override;
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
    Switch(int row, int column)
        : Tile(row, column, '?', ":/pics/textures/other tiles/switch.png") {}

    bool isEntrable() override { return true; }

    std::pair<bool, Tile *> onEnter(Character *who) override;
};

class Door : public Tile, public Passive
{
    bool isOpen{false};
    bool state{0};

    /* state 0 means wall, state 1 means floor*/
public:
    Door(int row, int column)
        : Tile(row, column, 'X', ":/pics/textures/doors/door1.png") {};
    void notify();
    bool isEntrable() { return state; }
};

class Pit : public Tile
{
    // TODO : bug, cant attack while on pit
public:
    Pit(int row, int column)
        : Tile(row, column, '_', ":/pics/textures/other tiles/pit.png") {}

    bool isEntrable() override { return true; }

    bool onLeave(Tile *desTile, Character *who) override;
};

class Ramp : public Tile
{
public:
    Ramp(int row, int column)
        : Tile(row, column, '<' , ":/pics/textures/other tiles/ramp.png") {};

    bool isEntrable() { return true; }
};

class LevelChanger : public Tile
{
public:
    LevelChanger(int row, int column)
        : Tile(row, column, '$', ":/pics/textures/extra/levelchanger.png") {};

    bool isEntrable() { return true; }
};

#endif //PRAK_TILE_H
