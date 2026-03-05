#include "HumanController.h"
#include "GameController.h"
HumanController::HumanController(Character* controlledCharacter)
    : AbstractController(controlledCharacter) {}

Coordinates HumanController::getNextMove() {
    Coordinates newTileCoordinates = GameController::GetLastTileClickedCords();
    return newTileCoordinates;
}
