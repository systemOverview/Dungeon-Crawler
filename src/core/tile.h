//
// Created by MBK on 12.11.25.
//
#ifndef PRAK_TILE_H
#define PRAK_TILE_H
#include <QtCore/qdebug.h>
#include "Character.h"
#include "Event.h"
#include "PassiveAktive.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;
class Tile
{
public:
protected:
    char m_texture;
    Coordinates m_coordinates;
    Character* character{nullptr};
    Tile(int row, int col, char texture);

public:
    static Tile* GenerateTile(char texture, int row, int column);
    char getTexture() const;
    Character *getCharacter() const;
    void setCharacter(Character* characterToPlace);
    int getRow() const;
    int getColumn() const;
    Coordinates getCoordinates() const;
    virtual ~Tile();
};

class Floor : public Tile
{ // Accessible tile, characters can enter/leave them

public:
    Floor(int row, int column)
        : Tile(row, column, '.') {};

    bool isEntrable() { return true; }
};

class Wall : public Tile
{ // Non-Accessible tile, characters can't enter them
public:
    Wall(int row, int column)
        : Tile(row, column, '#') {};

    bool isEntrable() { return false; }
};

class Portal : public Tile, public EventListener
{
    bool shouldMove{true};
    Portal* m_siblingPortal = nullptr;
    int m_portalId;

public:
    Portal(int row, int column, int portalId);


    void setPortal(Portal *portal);
    void setPortalId (int portalId);
    int getPortalId();
    Portal* getSiblingPortal();
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
    Switch(int row, int column)
        : Tile(row, column, '?') {}
};

class Door : public Tile, public Passive
{

    bool isOpen{false};
    bool state{0};    
    /* state 0 means wall, state 1 means floor*/
public:
    Door(int row, int column, bool isClosed = true)
        : Tile(row, column, 'X') {
        if (!isClosed){
            Door::notify();
        }
    };
    void notify();
};

class Pit : public Tile
{
public:
    Pit(int row, int column)
        : Tile(row, column, '_') {}
};

class Ramp : public Tile
{
public:
    Ramp(int row, int column)
        : Tile(row, column, '<') {};

    bool isEntrable() { return true; }
};

class LevelChanger : public Tile
{
public:
    LevelChanger(int row, int column)
        : Tile(row, column, '$') {};

    bool isEntrable() { return true; }
};

class GameWinner : public Tile{
public:
    GameWinner(int row, int column)
        : Tile(row, column, '!') {};
    bool isEntrable() { return true; }

};

void to_json(json &jsonObject, const Tile* tileObject);
#endif //PRAK_TILE_H
