#include "AttackController.h"
#include "LevelGraph.h"
#include "Level.h"


AttackController::AttackController(Character* controlledCharacter, Level* level, LevelGraph* graph)
    : AbstractController(controlledCharacter)
    , m_level{level}
    , m_graph{graph} {}

void AttackController::moveCharacter() {
    Coordinates newCords
        = Coordinates::CoordinatesSumClamped(m_controlledCharacter->getTile()->getCoordinates(),
                                             {0, 1},
                                             9,
                                             9);
    m_controlledCharacter->setTile(Level::GetTile(newCords));
}
