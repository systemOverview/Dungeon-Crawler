//
// Created by MBK on 13.11.25.
//

#include "DungeonCrawler.h"
#include <QApplication>
#include <QFile>
#include <QStandardPaths>
#include <QWindow>
#include <QtCore/qtimer.h>
#include "AbstractController.h"

// This class is follows the singleton pattern.
// It communicates mainly with the GUI:
//      (GUI alerts DC of game start -> DC builds the game -> DC gives game info to GUI)
//      (GUI alerts DC of user click on Tile -> DC alerts controllers to initiate their moves)
// It also keeps track of the game participants to avoid exposing them to the view or to each other as much as possible:
//      (ConnectGeneratedCharacter() connects DungeonCrawler::move to character.controller::moveCharacter)
//      (ConnectGeneratedCharacter() also connects the model character moving signals to its view,
//       with neither knowing about the other).

// It holds the game rules and validates them.
// Characters communicate through a static interface, they do not get ahold of the instance.
// The static methods call the instance when necessary (ie: signals/slots connections : ConnectGeneratedCharacter())

DungeonCrawler::DungeonCrawler() {
    assert(SINGLETON_INSTANCE == nullptr && "Multiple instanced of DC are being created");
    SINGLETON_INSTANCE = this;

    GUI = new GraphicalUI();

    connect(GUI, &GraphicalUI::gameStarted, this, &DungeonCrawler::buildGame);
    connect(GUI, &GraphicalUI::humanHasInitiatedMove, this, &DungeonCrawler::move);
}

void DungeonCrawler::buildGame() {
    // First, level is created, it creates all the tiles, and calls DC::CreateCharacter for characters creations
    // CreateCharacter creates the character and the controller too, connects the DC move signal to the controller move slot
    // It also asks GUI to create a view for the character.
    Level* level = new Level(GameStrings[0]);

    // Once that finishes, DC asks GUI to create the tiles UI items, and also add the previously created character to the scene.
    // GUI only adds the characters to the scene after tiles are created because characterItem::fixMyPosition requires the tile item
    // position.
    GUI->createLevelView(level->getTiles());
}

void DungeonCrawler::CreateCharacter(char letterRepresentingCharacterType, Tile* characterTile) {
    Character::CharacterType characterType = CHAR_TO_TYPE_DICTIONARY.at(
        letterRepresentingCharacterType);

    Character* characterModel = Character::GenerateCharacter(letterRepresentingCharacterType,
                                                             characterTile);

    AbstractController* characterController = AbstractController::CreateCharacterController(
        characterModel);

    CHARACTERS_CONTROLLERS.insert({characterModel, characterController});

    connect(SINGLETON_INSTANCE,
            &DungeonCrawler::move,
            characterController,
            &AbstractController::moveCharacter);
    CHARACTERS.push_back(characterModel);

    CharacterItem* characterView = GUI->createCharacterUI(characterModel->getCharacterType(),
                                                          characterTile->getCoordinates(),
                                                          characterModel->getCharacterID());

    connect(characterModel, &Character::characterMoved, GUI, &GraphicalUI::moveCharacterView);
    connect(characterModel, &Character::characterMoved, [characterModel]() {
        Portal* portal = dynamic_cast<Portal*>(characterModel->getTile());
        if (portal) {
            characterModel->setTileWithoutEmittingSignal(portal->getSiblingPortal());
            GUI->moveCharacterView(characterModel->getCharacterID(),
                                   portal->getCoordinates(),
                                   portal->getSiblingPortal()->getCoordinates());
        };
    });
}

bool DungeonCrawler::RequestMove(Character* character, Tile* wantedTile) {
    if (!IsTileInNeighbouringRange(character->getTile(), wantedTile)) return false;
    if (!IsTileToTileMovementLegal(character->getTile(), wantedTile)) return false;

    Character* characterAtWantedTile = WhoIsOccupyingTile(wantedTile);

    if (characterAtWantedTile != nullptr && AreCharactersEnemies(character, characterAtWantedTile)) {
        return AttemptForcedTileTakeover(character, characterAtWantedTile);
    }

    return true;
}

// RequestMove helpers

bool DungeonCrawler::IsTileInNeighbouringRange(Tile* from, Tile* to) {
    if (std::abs(from->getCoordinates().column - to->getCoordinates().column) > 1) return false;
    if (std::abs(from->getCoordinates().row - to->getCoordinates().row) > 1) return false;
    return true;
}

bool DungeonCrawler::IsTileToTileMovementLegal(Tile* from, Tile* to) {
    if (to->getTexture() == '#') return false;
    if ((from->getTexture() == '_' && (to->getTexture() != '!' && to->getTexture() != '<'))
        == true) {
        return false;
    }
    return true;
}

bool DungeonCrawler::AreCharactersEnemies(Character* firstCharacter, Character* secondCharacter) {
    return ((firstCharacter->getCharacterType() == Character::CharacterType::Human)
            != (secondCharacter->getCharacterType() == Character::CharacterType::Human));
}

Character* DungeonCrawler::WhoIsOccupyingTile(Tile* tile) {
    for (Character* character : CHARACTERS) {
        if (character->getTile() == tile) {
            return character;
        }
    }
    return nullptr;
}

// End of ValidateMove helpers

bool DungeonCrawler::AttemptForcedTileTakeover(Character* attacker, Character* defender) {
    bool didAttackerWin = true;

    FightEvent* fightEvent = new FightEvent();

    FightRound firstRound = HoldFightRound(attacker, defender);
    fightEvent->addFightRound(firstRound);

    if (firstRound.getFightRoundOutcome() != FightRound::FightRoundOutcome::DefenderKilled) {
        FightRound secondRound = HoldFightRound(defender, attacker); // inversed now
        fightEvent->addFightRound(secondRound);
        didAttackerWin = false;
    }

    QCoreApplication::postEvent(GUI, fightEvent);

    return didAttackerWin;
}

FightRound DungeonCrawler::HoldFightRound(Character* attacker, Character* defender) {
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

