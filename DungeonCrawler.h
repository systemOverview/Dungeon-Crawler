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
        "#........#"
        "#P.G?....#"
        "##########",

        "##########"
        "#.S..$...#"
        "#..G<....#"
        "#..___...#"
        "#..___...#"
        "#..P.G...#"
        "##########"
        "#G.......#"
        "#........#"
        "##########"

    };
    Character* m_humanCharacter;
    GraphicalUI *GUI;

public:
    DungeonCrawler();

    std::pair<int, int> translateMove(int step) const;
    bool turn();
    void move();
    void levelUp();
};

#endif //PRAK_DUNGEONCRAWLER_H
