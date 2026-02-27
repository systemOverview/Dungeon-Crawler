#include "StationaryController.h"
#include "Constants.h"

StationaryController::StationaryController(Character* controlledCharacter)
    : AbstractController(controlledCharacter) {}

Coordinates StationaryController::getNextMove() { return {0, 0}; }
