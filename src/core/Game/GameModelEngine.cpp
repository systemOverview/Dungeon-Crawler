#include "GameModelEngine.h"


//
// Created by MBK on 13.11.25.
//

#include <QApplication>
#include "AbstractController.h"
#include "Character.h"
#include "FightEvent.h"
#include "GameController.h"
#include "Level.h"

GameModelEngine::GameModelEngine() {}

void GameModelEngine::createGameFromString(std::string gameString) {
    CURRENT_LEVEL = new Level(gameString);

    for (Character* character : CURRENT_LEVEL->getCharacters()) {
        AbstractController* characterController = AbstractController::CreateCharacterController(
            character);
        characterController->setLevel(CURRENT_LEVEL);

        connect(this,
                &GameModelEngine::move,
                characterController,
                &AbstractController::moveCharacter);

        CHARACTERS_CONTROLLERS.insert({character, characterController});
    }
}

void GameModelEngine::createGameFromJson(json jsonInfo) {
    CURRENT_LEVEL = new Level(jsonInfo);

    for (Character* character : CURRENT_LEVEL->getCharacters()) {
        AbstractController* characterController = AbstractController::CreateCharacterController(
            character);
        characterController->setLevel(CURRENT_LEVEL);

        connect(this,
                &GameModelEngine::move,
                characterController,
                &AbstractController::moveCharacter);

        CHARACTERS_CONTROLLERS.insert({character, characterController});
    }
}

bool GameModelEngine::RequestMove(Character* character, Tile* wantedTile) {
    if (!IsTileInNeighbouringRange(character->getTile(), wantedTile)) return false;
    if (!IsTileToTileMovementLegal(character->getTile(), wantedTile)) return false;

    Character* characterAtWantedTile = CURRENT_LEVEL->whoIsOccupyingTile(wantedTile);

    if (characterAtWantedTile != nullptr && AreCharactersEnemies(character, characterAtWantedTile)) {
        return AttemptForcedTileTakeover(character, characterAtWantedTile);
    }

    return true;
}

Level* GameModelEngine::getCurrentLevel() { return CURRENT_LEVEL; }

// RequestMove helpers

bool GameModelEngine::IsTileInNeighbouringRange(Tile* from, Tile* to) {
    if (std::abs(from->getCoordinates().column - to->getCoordinates().column) > 1) return false;
    if (std::abs(from->getCoordinates().row - to->getCoordinates().row) > 1) return false;
    return true;
}

bool GameModelEngine::IsTileToTileMovementLegal(Tile* from, Tile* to) {
    if (to->getTileType() == Types::TileType::Wall) return false;
    if ((from->getTileType() == Types::TileType::Pit
         && (to->getTileType() != Types::TileType::Pit
             && to->getTileType() == Types::TileType::Ramp))
        == true) {
        return false;
    }
    return true;
}

bool GameModelEngine::AreCharactersEnemies(Character* firstCharacter, Character* secondCharacter) {
    return ((firstCharacter->getCharacterType() == Types::CharacterType::Human)
            != (secondCharacter->getCharacterType() == Types::CharacterType::Human));
}

// End of ValidateMove helpers

bool GameModelEngine::AttemptForcedTileTakeover(Character* attacker, Character* defender) {
    bool didAttackerWin = true;

    FightEvent* fightEvent = new FightEvent();

    FightRound firstRound = HoldFightRound(attacker, defender);
    fightEvent->addFightRound(firstRound);

    if (firstRound.getFightRoundOutcome() != FightRound::FightRoundOutcome::DefenderKilled) {
        FightRound secondRound = HoldFightRound(defender, attacker); // inversed now
        fightEvent->addFightRound(secondRound);
        didAttackerWin = false;
    }

    // QCoreApplication::postEvent(GUI, fightEvent);

    return didAttackerWin;
}

FightRound GameModelEngine::HoldFightRound(Character* attacker, Character* defender) {
    defender->decrementFromHealthPoints(attacker->getStrength());

    const FightRound::CharacterInfo attackerPostRoundInfo = {attacker->getCharacterID(),
                                                             attacker->getCurrentHealthPoints()};

    const FightRound::CharacterInfo defenderPostRoundInfo
        = {defender->getCharacterID(),
           defender->getCurrentHealthPoints()}; // save these info because the defender might get deleted next.

    FightRound::FightRoundOutcome fightRoundOutcome{FightRound::FightRoundOutcome::Draw};

    if (defender->isAlive() == false) {
        fightRoundOutcome = FightRound::FightRoundOutcome::DefenderKilled;

        AbstractController* defenderController = CHARACTERS_CONTROLLERS.at(defender);
        CHARACTERS_CONTROLLERS.erase(defender);
        delete defenderController;
        defenderController = nullptr;

        delete defender;
        defender = nullptr;
    }

    FightRound fightRound{attackerPostRoundInfo, defenderPostRoundInfo, fightRoundOutcome};

    return fightRound;
}

void GameModelEngine::askCharactersToMove() { emit move(); }
