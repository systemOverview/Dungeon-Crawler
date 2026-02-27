#include "GuardController.h"
void GuardController::moveCharacter() {}

GuardController::GuardController(Character* controlledCharacter)
    : AbstractController(controlledCharacter) {
    m_movementPathIterator = m_movementPath.begin();
}

std::pair<int, int> GuardController::move()
{
    if (m_movementPathIterator == m_movementPath.end()) {
        m_movementPathIterator = m_movementPath.begin();
    }
    std::pair<int, int> xymove = translateMove(*m_movementPathIterator);

    return xymove;
}
