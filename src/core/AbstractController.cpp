#include "AbstractController.h"
#include "AttackController.h"
#include "GuardController.h"
#include "HumanController.h"
#include "StationaryController.h"

#include "Character.h"

AbstractController::AbstractController(Character* controlledCharacter)
    : m_controlledCharacter(controlledCharacter) {}

AbstractController* AbstractController::CreateCharacterController(Character* character) {
    switch (character->getCharacterType()) {
    case (Character::CharacterType::Human): {
        return new HumanController(character);
    }
    case (Character::CharacterType::Attacker): {
        return new AttackController(character);
    }
    case Character::CharacterType::StationaryZombie: {
        return new StationaryController(character);
    }
    case Character::CharacterType::GuardZombie: {
        return new GuardController(character);
    };
    }
}
