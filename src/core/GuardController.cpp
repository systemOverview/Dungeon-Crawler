#include "GuardController.h"
#include "Constants.h"

GuardController::GuardController(Character* controlledCharacter)
    : AbstractController(controlledCharacter) {
}

Coordinates GuardController::getNextMove() { return {0, 0}; }
