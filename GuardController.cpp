#include "GuardController.h"
GuardController::GuardController() {
    m_movementPathIterator = m_movementPath.begin();
}

int GuardController::move(Level *currentLevel)
{
    if (m_movementPathIterator == m_movementPath.end()){
        m_movementPathIterator = m_movementPath.begin();
    }
    Tile* currentTile = m_character->getTile();

    std::pair<int,int> xymove = translateMove(*m_movementPathIterator);
    int newRow = currentTile->getRow()+xymove.first;
    int newColumn = currentTile->getColumn()+xymove.second;
    Tile* wantedTile = currentLevel->getTile(newRow, newColumn);
    bool isMoveAllowed = currentTile->moveTo(wantedTile,m_character);
    m_movementPathIterator++;

    return 5;
}
