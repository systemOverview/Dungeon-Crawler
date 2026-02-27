//
// Created by MBK on 13.11.25.
//

#include "DungeonCrawler.h"
#include <QApplication>
#include <QFile>
#include <QStandardPaths>
#include <QWindow>
#include <QtCore/qtimer.h>

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
    GUI = new GraphicalUI();
    SINGLETON_INSTANCE = this;

    connect(GUI, &GraphicalUI::gameStarted, this, &DungeonCrawler::buildGame);
    connect(GUI, &GraphicalUI::humanHasInitiatedMove, this, &DungeonCrawler::move);
}

void DungeonCrawler::buildGame() {
    Level* level = new Level(GameStrings[0]);
    GUI->createLevelUI(level->getTiles());
}

void DungeonCrawler::ConnectGeneratedCharacter(Character* characterModel,
                                               CharacterItem* characterView) {
    // model to controller connection
    AbstractController* controller = characterModel->getController();
    connect(SINGLETON_INSTANCE,
            &DungeonCrawler::move,
            controller,
            &AbstractController::moveCharacter);

    // model to view connection
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
