//
// Created by MBK on 13.11.25.
//

#include "Level.h"
#include "StationaryController.h"
#include "GuardController.h"

Level::Level(int height, int width, std::string gameString) {
    gameHeight = height;
    gameWidth = width;
    setDefaultTiles();

    std::vector <Door*> doors;
    Switch* theSwitch = nullptr;




    for (int i=0; i<gameString.length();i++) {
        int row = i/10;
        int column = i%10;
        if (gameString[i]=='#') {
            Wall* wall = new Wall(row, column);
            (tiles)[row][column] = wall;
        }

        else if (gameString[i]=='X') {
            Door* door = new Door(row, column);
            doors.push_back(door);
            (tiles)[row][column] = door;
        }

        else if (gameString[i]=='?') {
            Switch* switcher = new Switch(row, column);
            (tiles)[row][column] = switcher;
            theSwitch = switcher;
        }

        else if (gameString[i]=='_') {
            Pit* pit = new Pit(row, column);
            (tiles)[row][column] = pit;
        }


        else if (gameString[i]=='<') {
            Ramp* ramp = new Ramp(row, column);
            (tiles)[row][column] = ramp;
        }

        else if (gameString[i]=='$') {
            LevelChanger* levelChanger = new LevelChanger(row, column);
            (tiles)[row][column] = levelChanger;
        }

        else {
            Floor* floor = new Floor(row, column);
            (tiles)[row][column] = floor;

            if (gameString[i]=='P'){
                Character* crc = new Character("P","/pics/ textures/char/front/char_front_1.png");
                placeCharacter(crc,row,column,true);

            }

            else if (gameString[i]=='S'){
                StationaryController* stationaryController = new StationaryController;
                Character* crc = new Zombie("S","/pics/textures/zombie/zombie_right.png",stationaryController);
                placeCharacter(crc,row,column,false);

            }

            else if (gameString[i]=='G'){
                GuardController* guardController = new GuardController;
                Character* crc = new Zombie("G","/pics/textures/zombie/assasin.png",guardController);
                placeCharacter(crc,row,column,false);

            }


        }

    }
    setPortals();
    // Character* crc = new Character("P","/pics/ textures/char/front/char_front_1.png");
    // placeCharacter(crc,9,2,true);


    for (auto door : doors) {
        theSwitch->attach(door);
    }

}

Tile* Level::getTile(int row, int col) {
    Tile* tileToReturn = tiles[row][col];
    return tileToReturn;
}

std::vector<std::vector<Tile *>>* Level::getTiles() {
    return &tiles;
}

int Level::getHeight() const {
    return gameHeight;
}

int Level::getWidth() const {
    return gameWidth;
}

void Level::placeCharacter(Character* c, int row, int col, bool isPlayable) {
    Tile* tileToPlaceAt = tiles[row][col];
    tileToPlaceAt->setCharacter(c);
    c->setTile(tileToPlaceAt);
    characters.push_back(c);
    if (isPlayable) {
        playableCharacter = c;
    }
    else{
        nonPlayableCharacters.push_back(c);
    }
}

std::vector<Character *> Level::getNonPlayableCharacters()
{
    return nonPlayableCharacters;
}

void Level::setPortals()
{
    delete (tiles)[3][2];
    delete (tiles)[5][7];
    Portal* firstPortal = new Portal(3,2);
    Portal* secondPortal = new Portal(5,7);
    (tiles)[3][2] = firstPortal;
    (tiles)[5][7] = secondPortal;
    firstPortal->setPortal(secondPortal);
    secondPortal->setPortal(firstPortal);

}
