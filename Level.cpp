//
// Created by MBK on 13.11.25.
//

#include "Level.h"
#include "GuardController.h"
#include "StationaryController.h"
#include "AttackController.h"
#include <qDebug>

Level::Level(int height, int width, std::string gameString)
{
    gameHeight = height;
    gameWidth = width;
    setDefaultTiles();

    std::vector<Door *> doors;
    Switch *theSwitch = nullptr;

    for (int i = 0; i < gameString.length(); i++) {
        int row = i / 10;
        int column = i % 10;
        if (gameString[i] == '#') {
            Wall *wall = new Wall(row, column);
            (tiles)[row][column] = wall;
        }

        else if (gameString[i] == 'X') {
            Door *door = new Door(row, column);
            doors.push_back(door);
            (tiles)[row][column] = door;
            m_graph->addVertex(door, 1);
        }

        else if (gameString[i] == '?') {
            Switch *switcher = new Switch(row, column);
            (tiles)[row][column] = switcher;
            theSwitch = switcher;
            m_graph->addVertex(switcher, 1);
        }

        else if (gameString[i] == '_') {
            Pit *pit = new Pit(row, column);
            (tiles)[row][column] = pit;
            m_graph->addVertex(pit, 1);
        }

        else if (gameString[i] == '<') {
            Ramp *ramp = new Ramp(row, column);
            (tiles)[row][column] = ramp;
            m_graph->addVertex(ramp, 1);
        }

        else if (gameString[i] == '$') {
            LevelChanger *levelChanger = new LevelChanger(row, column);
            (tiles)[row][column] = levelChanger;
        }

        else {
            Floor *floor = new Floor(row, column);
            (tiles)[row][column] = floor;
            m_graph->addVertex(floor, 1);

            if (gameString[i] == 'P') {
                m_playingCharacterPosition = {row, column};
            }

            else if (gameString[i] == 'S') {
                StationaryController *stationaryController = new StationaryController;
                Character *crc = new Zombie("S",
                                            "/pics/textures/zombie/zombie_right.png",
                                            10,
                                            5,
                                            stationaryController);
                placeCharacter(crc, row, column, false);

            }

            else if (gameString[i] == 'G') {
                GuardController *guardController = new GuardController;
                Character *crc = new Zombie("G",
                                            "/pics/textures/zombie/assassin.png",
                                            10,
                                            5,
                                            guardController);
                placeCharacter(crc, row, column, false);
            }

            else if (gameString[i] == 'A') {
                AttackController* attackController = new AttackController;
                Character *crc = new Attacker("A",
                                            "/pics/textures/zombie/attacker.png",
                                            10,
                                            5,
                                            attackController);
                placeCharacter(crc, row, column, false);
            }
        }
    }
    setPortals();
    m_graph->setupAlldges();
    for (auto door : doors) {
        theSwitch->attach(door);
    }
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
    return gameHeight;
}

int Level::getWidth() const
{
    return gameWidth;
}

void Level::placeCharacter(Character *c, int row, int col, bool isPlayable)
{
    Tile *tileToPlaceAt = tiles[row][col];
    tileToPlaceAt->setCharacter(c);
    c->setTile(tileToPlaceAt);
    characters.push_back(c);
    if (isPlayable) {
        playableCharacter = c;
    } else {
        nonPlayableCharacters.push_back(c);
    }
}

void Level::placePlayingCharacter(Character *c)
{
    // this function is called by the dungeon controller, since the playing character is the same across all levels
    // everytime a player levels up, the controller calls this to place the character. The other characters (NPCs)
    // don't move across levels.

    placeCharacter(c, m_playingCharacterPosition.first, m_playingCharacterPosition.second, true);
}

Character *Level::getPlayableCharacter()
{
    return playableCharacter;
}

std::vector<Character *> Level::getNonPlayableCharacters()
{
    return nonPlayableCharacters;
}

void Level::setPortals()
{
    delete (tiles)[3][2];
    delete (tiles)[5][7];
    Portal *firstPortal = new Portal(3, 2);
    Portal *secondPortal = new Portal(5, 7);
    (tiles)[3][2] = firstPortal;
    (tiles)[5][7] = secondPortal;
    firstPortal->setPortal(secondPortal);
    secondPortal->setPortal(firstPortal);
    m_graph->getVertex({3,2})->setTile(firstPortal);
    m_graph->getVertex({5,7})->setTile(secondPortal);
}

void Level::setDefaultTiles()
{
    for (int i = 0; i < gameHeight; i++) {
        std::vector<Tile *> row;
        tiles.push_back(row);
        for (int j = 0; j < gameWidth; j++) {
            tiles[i].push_back(nullptr);
        }
    }
}
