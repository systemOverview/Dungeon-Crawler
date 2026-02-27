#include "HumanController.h"
#include "DungeonCrawler.h"
#include "GraphicalUI.h"

HumanController::HumanController(Character* controlledCharacter)
    : AbstractController(controlledCharacter) {}

Coordinates HumanController::getNextMove() {
    Coordinates newTileCoordinates = GraphicalUI::GetLastTileClickedCords();
    return newTileCoordinates;
}
