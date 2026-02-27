#include "HumanController.h"
#include "GraphicalUI.h"
void HumanController::moveCharacter() {
    Coordinates newTileCoordinates = GraphicalUI::GetLastTileClickedCords();
    m_controlledCharacter->setTile(Level::GetTile(newTileCoordinates));
}

HumanController::HumanController(Character* controlledCharacter)
    : AbstractController(controlledCharacter) {}

std::pair<int, int> HumanController::move() {}
