//
// Created by MBK on 12.11.25.
//

#ifndef PRAK_TILE_H
#define PRAK_TILE_H

#include <iostream>
#include "Character.h"
#include "PassiveAktive.h"
class Tile {
protected:

    std::string texture;
    Character* character {nullptr}; // Character on top of the tile, or nullptr if none.
    int row{};
    int column{};
    bool shouldMove{false}; // Should the player be moved to another tile (true if tile is portal)
    std::string texturePath;



    Tile(std::string Texture, int rw, int cln, std::string textureP) {
        texture = Texture;
        row = rw;
        column = cln;
        texturePath = textureP;

    }


public:

    /*
    virtual void doNothing() = 0;
    */
    virtual ~Tile() = default;
    virtual std::string_view getTexture() const;
    Character* getCharacter() const;
    bool hasCharacter() const;
    void setCharacter(Character* characterToPlace);
    int getRow();
    int getColumn();

    virtual bool moveTo (Tile* desTile, Character* who);
    virtual bool onLeave(Tile* desTile, Character* who);
    virtual std::pair<bool, Tile*> onEnter(Character* who);
    virtual std::string getTexturePath();
    virtual bool isEntrable()  = 0;
};

class Floor : public Tile { // Accessible tile, characters can enter/leave them
    /*void doNothing() {
        int a = 6;
    }*/
    std::string texture {"."};

public:
    Floor(int row, int column) :Tile(".", row, column, ":/pics/textures/floor/floor1.png") {
    };


    bool isEntrable() {
        return true;
    }
};

class Wall : public Tile { // Non-Accessible tile, characters can't enter them
    /*void doNothing() {
        int a = 16;
    }*/


public:
    Wall(int row, int column) :Tile("#", row, column, ":/pics/textures/wall/wall1.png") {

    };

    bool isEntrable() {
        std::cout << "wall";
        return false;
    }

    virtual std::string getTexturePath();
};


class Portal : public Tile {
    /*void doNothing() {
        int a = 36;
    }*/
    bool shouldMove{true};
    Portal* portalToAccess;
    int portalType;
public:
    Portal(int row, int column) :Tile("O", row, column, ":/pics/textures/portal/portal1.png") {

    };

    bool isEntrable() {
        return true;
    }

    void setPortal(Portal* portal) {
        portalToAccess = portal;
    }

    std::pair<bool,Tile*> onEnter(Character *who) override;



};

class basePlate : public Tile{
    bool isOpen {false};
public:
    void switchStatus (bool status);
};

class Switch : public Tile, public Active {
public:
    Switch(int row, int column) :Tile("?", row, column ,":/pics/switch.png") {

    };

    bool isEntrable() {
        return true;
    }

    std::pair<bool,Tile*> onEnter(Character *who) override;

};

class Door : public Tile, public Passive {
    bool isOpen {false};
    bool state {0};

     /* state 0 means wall, state 1 means floor*/
public:
    Door(int row, int column) :Tile("X", row, column, ":/pics/textures/doors/door1.png") {

    };
    void notify();
    bool isEntrable() {
        return state;
    }

};


class Pit : public Tile {

public:
    Pit(int row, int column) :Tile("_", row, column, ":/pics/textures/pit.png") {

    };

    bool isEntrable() {
        return true;
    }



    bool onLeave(Tile *desTile, Character *who) override;
};

class Ramp : public Tile {

public:
    Ramp(int row, int column) :Tile("<", row, column, ":/pics/textures/ramp.png") {

    };

    bool isEntrable() {
        return true;
    }
};

class LevelChanger : public Tile{
public:
    LevelChanger(int row, int column) :Tile("$", row, column, ":/pics/textures/levelchanger.png") {

    };

    bool isEntrable() {
        return true;
    }

};

#endif //PRAK_TILE_H
