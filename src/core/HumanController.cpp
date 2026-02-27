#include "HumanController.h"
#include "DungeonCrawler.h"
#include "GraphicalUI.h"

void HumanController::moveCharacter() {
    Coordinates newTileCoordinates = GraphicalUI::GetLastTileClickedCords();
    DungeonCrawler::RequestMove(m_controlledCharacter, Level::GetTile(newTileCoordinates));
    m_controlledCharacter->setTile(Level::GetTile(newTileCoordinates));
}

HumanController::HumanController(Character* controlledCharacter)
    : AbstractController(controlledCharacter) {}

