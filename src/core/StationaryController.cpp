#include "StationaryController.h"
void StationaryController::moveCharacter() {}

StationaryController::StationaryController(Character* controlledCharacter)
    : AbstractController(controlledCharacter) {}

std::pair<int, int> StationaryController::move()
{
    return {0, 0};
}
