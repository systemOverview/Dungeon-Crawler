//
// Created by MBK on 13.11.25.
//

#ifndef PRAK_DUNGEONCRAWLER_H
#define PRAK_DUNGEONCRAWLER_H
#include "GraphicalUI.h"
#include "Level.h"
#include "TerminalUI.h"
#include "list.h"
class DungeonCrawler
{
public:
    List<Level*>::Iterator currentLevel;
    List<Level *> levels;
    std::array<std::string, 2> gameStrings = {

        "##########"
        "#....$...#"
        "#...<..S.#"
        "#..___...#"
        "#..___...#"
        "#G.......#"
        "#######X##"
        "#...G....#"
        "#P..?....#"
        "##########",

        "##########"
        "#.S..$...#"
        "#...<....#"
        "#..___...#"
        "#..___...#"
        "#........#"
        "#######X##"
        "#G.......#"
        "#P.?.....#"
        "##########"

    };
    GraphicalUI *GUI;

public:
    DungeonCrawler();

    std::pair<int, int> translateMove(int step) const;
    bool turn();
    void move();
};

#endif //PRAK_DUNGEONCRAWLER_H
