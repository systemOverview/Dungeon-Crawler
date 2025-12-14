#include "StationaryController.h"
#include "tile.h"
// #include "Level.cpp"
StationaryController::StationaryController()
{

}

void StationaryController::attachCharacter(Character* character)
{
    m_character = character;
}

int StationaryController::move(Level* currentLevel)
{

    Tile* currentTile = m_character->getTile();
    // int move = character->move(currentLevel);
    // std::pair <int,int> xymove = translateMove(move);


    int newRow = currentTile->getRow()+1;
    int newColumn = currentTile->getColumn()+0;
    Tile* wantedTile = currentLevel->getTile(newRow, newColumn);
    bool isMoveAllowed = currentTile->moveTo(wantedTile,m_character);

    return 5;
}
