//
// Created by MBK on 12.11.25.
//
#ifndef PRAK_TILE_H
#define PRAK_TILE_H
#include <QtCore/qdebug.h>
#include "Character.h"
#include "Event.h"
#include "PassiveAktive.h"
#include "Types.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;
class Tile : public QObject
{
    Q_OBJECT
public:

protected:
    Types::TileType m_tileType;
    Coordinates m_coordinates;
    Character* character{nullptr};
    Tile(Types::TileType tileType, int row, int col);

signals:
    void tileTypeChanged(Types::TileType tileType);

public:
    static Tile* GenerateTile(Types::TileType tileType, int row, int column);
    static Tile* GenerateTile(Types::TileType tileType, Coordinates coordinates);

    Types::TileType getTileType() const;
    Character *getCharacter() const;
    void setCharacter(Character* characterToPlace);
    int getRow() const;
    int getColumn() const;
    Coordinates getCoordinates() const;
    virtual void alertOfAccess() {};
    virtual ~Tile();
};

class Floor : public Tile
{
public:
    Floor(int row, int column)
        : Tile(Types::TileType::Floor, row, column) {};

    bool isEntrable() { return true; }
};

class Wall : public Tile
{ // Non-Accessible tile, characters can't enter them
public:
    Wall(int row, int column)
        : Tile(Types::TileType::Wall, row, column) {};

    bool isEntrable() { return false; }
};

class Portal : public Tile, public EventListener
{
    inline static std::multimap<int, Portal*> PORTALS;

    bool shouldMove{true};
    Portal* m_siblingPortal = nullptr;
    int m_portalId;

public:
    Portal(int row, int column);

    void setPortal(Portal* portal);
    void setPortalId (int portalId);
    int getPortalId();
    Portal* getSiblingPortal();
    void onPortalCreation(PortalCreationEvent* event) override;
    void setSiblingPortal(Portal* newSiblingPortal);
};

class Switch : public Tile, public Active
{

public:
    Switch(int row, int column)
        : Tile(Types::TileType::Switch, row, column) {}
};

class Door : public Tile
{
public:
    Door(Types::TileType doorType, int row, int column)
        : Tile(doorType, row, column) {
        assert((doorType == Types::TileType::OpenDoor || doorType == Types::TileType::ClosedDoor)
               && "Type supplied is not a door.");
    }
    void alertOfAccess() override;
};

class Pit : public Tile
{
public:
    Pit(int row, int column)
        : Tile(Types::TileType::Pit, row, column) {}
};

class Ramp : public Tile
{
public:
    Ramp(int row, int column)
        : Tile(Types::TileType::Ramp, row, column) {};

    bool isEntrable() { return true; }
};

class LevelChanger : public Tile
{
public:
    LevelChanger(int row, int column)
        : Tile(Types::TileType::LevelChanger, row, column) {};

    bool isEntrable() { return true; }
};

class GameWinner : public Tile{
public:
    GameWinner(int row, int column)
        : Tile(Types::TileType::GameWinner, row, column) {};
    bool isEntrable() { return true; }

};

// void to_json(json &jsonObject, const Tile* tileObject);
#endif //PRAK_TILE_H
