//
// Created by MBK on 13.11.25.
//

#include "TerminalUI.h"
#include "Level.h"
class Tile;

void TerminalUI::draw(Level *level)
{
    std::vector<std::vector<Tile *>> *tiles = level->getTiles();
    for (int i = 0; i < level->getHeight(); i++) {
        for (int j = 0; j < level->getWidth(); j++) {
            Tile *currentTile = (*tiles)[i][j];
            if (currentTile == nullptr) {
                std::cout << "!";
            }

            else {
                if (currentTile->hasCharacter()) {
                    std::cout << currentTile->getCharacter()->getTexture();
                } else {
                    std::cout << currentTile->getTexture();
                }
            }
        }
        std::cout << std::endl;
    }
}

std::pair<int, int> TerminalUI::move()
{
    int movement{};
    std::cout << "Please enter your move (1-9)";
    std::cin >> movement;
    std::cout << '\n';
    return translateMove(movement);
}

std::pair<int, int> TerminalUI::translateMove(int step)
{
    std::pair<int, int> xymove;
    switch (step) {
        /* to go down, you add +1 to the row, to go up -1 the row, right +1 the column, left -1 the column*/
    case 1:
        xymove = {1, -1};
        return xymove;
    case 2:
        xymove = {1, 0};
        return xymove;
    case 3:
        xymove = {1, 1}; /*1-3, 2,2*/
        return xymove;
    case 4:
        xymove = {0, -1};
        return xymove;
    case 5:
        xymove = {0, 0};
        return xymove;
    case 6:
        xymove = {0, 1};
        return xymove;
    case 7:
        xymove = {-1, -1};
        return xymove;
    case 8:
        xymove = {-1, 0};
        return xymove;
    case 9:
        xymove = {-1, 1};
        return xymove;
    }
}
