#include "AttackController.h"
#include "LevelGraph.h"
#include "Level.h"

AttackController::AttackController(Character* controlledCharacter)
    : AbstractController(controlledCharacter) {}

void AttackController::moveCharacter() {
    Coordinates newCords
        = Coordinates::CoordinatesSumClamped(m_controlledCharacter->getTile()->getCoordinates(),
                                             {0, 1},
                                             9,
                                             9);
    m_controlledCharacter->setTile(Level::GetTile(newCords));
}
