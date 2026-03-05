#include "AbstractController.h"
#include "AttackController.h"
#include "Character.h"
#include "GuardController.h"
#include "HumanController.h"
#include "StationaryController.h"

#include <GameModelEngine.h>
#include <Level.h>

void AbstractController::setLevel(Level* level) { m_level = level; }

void AbstractController::moveCharacter() {
    Coordinates newTileCoordinates = getNextMove();
    Tile* newTile = m_level->getTile(newTileCoordinates);
    if (GameModelEngine::RequestMove(m_controlledCharacter, newTile)) {
        m_controlledCharacter->setTile(newTile);
    };
}

AbstractController::AbstractController(Character* controlledCharacter)
    : m_controlledCharacter(controlledCharacter) {}

AbstractController* AbstractController::CreateCharacterController(Character* character) {
    switch (character->getCharacterType()) {
    case (Types::CharacterType::Human): {
        return new HumanController(character);
    }
    case (Types::CharacterType::Attacker): {
        return new AttackController(character);
    }
    case Types::CharacterType::StationaryZombie: {
        return new StationaryController(character);
    }
    case Types::CharacterType::GuardZombie: {
        return new GuardController(character);
    };
    }
}
