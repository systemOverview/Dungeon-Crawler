#include <iostream>
#include "header.h"
#include "Tile.h"
#include "Level.h"
#include "AbstractUI.h"
#include "DungeonCrawler.h"
#include "TerminalUI.h"
#include "startscreen.h"
#include "GraphicalUI.h"
#include <QApplication>




// Level generateLevel() {
//     Level level(10,10);
//     std::vector <Door*> doors;
//     Switch* theSwitch = nullptr;


//     auto tiles = level.getTiles();

//     const std::string gameString = {
//         "##########"
//         "#........#"
//         "#...<....#"
//         "#..___...#"
//         "#..___...#"
//         "#........#"
//         "#######X##"
//         "#........#"
//         "#...?....#"
//         "##########"
//     };

//     for (int i=0; i<gameString.length();i++) {
//         int row = i/10;
//         int column = i%10;
//         if (gameString[i]=='#') {
//             Wall* wall = new Wall(row, column);
//             (*tiles)[row][column] = wall;
//         }

//         else if (gameString[i]=='X') {
//             Door* door = new Door(row, column);
//             doors.push_back(door);
//             (*tiles)[row][column] = door;
//         }

//         else if (gameString[i]=='?') {
//             Switch* switcher = new Switch(row, column);
//             (*tiles)[row][column] = switcher;
//             theSwitch = switcher;
//         }

//         else if (gameString[i]=='_') {
//             Pit* pit = new Pit(row, column);
//             (*tiles)[row][column] = pit;
//         }


//         else if (gameString[i]=='<') {
//             Ramp* ramp = new Ramp(row, column);
//             (*tiles)[row][column] = ramp;
//         }

//         else {
//             Floor* floor = new Floor(row, column);
//             (*tiles)[row][column] = floor;

//         }

//     }
//     delete (*tiles)[3][2];
//     delete (*tiles)[5][7];
//     Portal* firstPortal = new Portal(3,2);
//     Portal* secondPortal = new Portal(5,7);
//     (*tiles)[3][2] = firstPortal;
//     (*tiles)[5][7] = secondPortal;
//     Character* crc = new Character("P");
//     level.placeCharacter(crc,2,2,true);

//     firstPortal->setPortal(secondPortal);
//     secondPortal->setPortal(firstPortal);

//     for (auto door : doors) {
//         theSwitch->attach(door);
//     }

//     return level;


// }

// // int main() {

// //     Level level (10,10);
// //     /*
// //     AbstractUI *terminal = level.getPlayableCharacter()->getTerminal();

// //     TerminalUI *ter = dynamic_cast<TerminalUI *>(terminal);
// //     */
// //     DungeonCrawler dc (&level);
// //     while(true){
// //         bool isRunning = dc.turn();
// //         if (!isRunning){
// //             break;
// //         }

// //     }


// // }
#include "list.h"
#include "list.cpp"
int main(int argc, char *argv[])
{
    // List<int> integers;
    // integers.push_back(5);
    // integers.push_back(6);
    // integers.push_back(7);

    // for (auto it = integers.begin(), end = integers.end(); it != end; ++it) {
    //     auto i = *it;
    //     std::cout << i.getValue() << "\n";
    // }

    QApplication a(argc, argv);

    DungeonCrawler dc;
    // GraphicalUI graphicalUI;
    // QMainWindow w;
    // StartScreen w;
    // graphicalUI.getStartScreen()->show();
    return a.exec();
}
