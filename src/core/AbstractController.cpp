#include "AbstractController.h"
#include "Character.h"

AbstractController::AbstractController(Character* controlledCharacter)
    : m_controlledCharacter(controlledCharacter) {}

