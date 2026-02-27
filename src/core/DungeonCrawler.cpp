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
    GUI->createLevelUI(level->getTiles());
}

void DungeonCrawler::CreateCharacter(char characterIdentifier, Tile* characterTile) {
    Character::CharacterType characterType = CHAR_TO_TYPE_DICTIONARY.at(characterIdentifier);

    Character* characterModel = new Character(characterType, characterTile);
    characterTile->setCharacter(characterModel);

    AbstractController* characterController = AbstractController::CreateCharacterController(
        characterModel);

    if (characterModel->getCharacterType() == Character::CharacterType::Human) {
        CHARACTERS_CONTROLLERS.insert(CHARACTERS_CONTROLLERS.begin(), characterController);
    }
    else {
        CHARACTERS_CONTROLLERS.push_back(characterController);
    }

    qDebug() << characterIdentifier << int(characterModel->getCharacterType());

    CharacterItem* characterView = GUI->createCharacterUI(characterModel->getCharacterType(),
                                                          characterTile->getCoordinates());
    connect(characterModel, &Character::moved, characterView, &CharacterItem::AnimateMove);
}

bool DungeonCrawler::RequestMove(Character* character, Tile* tile) {
    if (tile->getCharacter()) {
    }
    return true;
}

bool DungeonCrawler::ValidateMove(Tile* from, Tile* to) {
    if (!IsTileInNeighbouringRange(from, to)) return false;
    return true;
}

// ValidateMove helpers

bool DungeonCrawler::IsTileInNeighbouringRange(Tile* from, Tile* to) {
    if (std::abs(from->getCoordinates().column - to->getCoordinates().column) > 1) return false;
    if (std::abs(from->getCoordinates().row - to->getCoordinates().row) > 1) return false;
    return true;
}

// End of ValidateMove helpers

//
