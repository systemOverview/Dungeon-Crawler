#include "AbstractUI.h"
void AbstractController::attachCharacter(Character *character)
{
    m_character = character;
}

std::pair<int, int> AbstractController::translateMove(int step)
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

std::pair<int, int> AbstractUI::translateMove(int step)
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
