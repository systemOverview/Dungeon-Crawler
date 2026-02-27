//
// Created by MBK on 13.11.25.
//

#include "DungeonCrawler.h"
#include <QApplication>
#include <QFile>
#include <QStandardPaths>
#include <QWindow>
#include <QtCore/qtimer.h>
#include "startscreen.h"
#include <fstream>

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
    m_GUI = new GraphicalUI();
    SINGLETON_INSTANCE = this;

    connect(m_GUI, &GraphicalUI::gameStarted, this, &DungeonCrawler::buildGame);
    connect(m_GUI, &GraphicalUI::humanHasInitiatedMove, this, &DungeonCrawler::move);
}

void DungeonCrawler::buildGame() {
    Level* level = new Level(gameStrings[0]);
    m_levels.push_back(level);
    m_GUI->createLevelUI(level->getTiles());
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

//old

void DungeonCrawler::build(GameSourceOption option) {
    if (option == GameSourceOption::FromStrings) {
        for (std::string& gameString : gameStrings) {
            Level* lvl = new Level(10, 10, gameString, true);
            m_levels.push_back(lvl);
            m_lastLevel = lvl;
        }
    }
    else {
        QString storagePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        std::string filePath = storagePath.toStdString() + "/level.json";
        std::ifstream file(filePath);
        json parsedJson = json::parse(file);
        std::cout << parsedJson;
        for (const auto& level : parsedJson["Levels"]) {
            Level* lvl = new Level(level);
            m_levels.push_back(lvl);
            m_lastLevel = lvl;
        }
    }

    m_startScreen->hide();

    (*currentLevel)->activateLevel();
}

std::pair<int, int> DungeonCrawler::translateMove(int step) const {
    std::pair<int, int> xymove;

    switch (step) {
    /* to go down, you add +1 to the row, to go up -1 the row, right +1 the column, left -1 the column*/
    case 1:
        xymove = {1, -1};
        return xymove;
    case 2:
        xymove = {1, 0};
        return xymove;
    case 3:
        xymove = {1, 1};
        return xymove;
    case 4:
        xymove = {0, -1};
        return xymove;
    case 5:
        xymove = {0, 0};
        return xymove;
    case 6:
        xymove = {0, 1};
        return xymove;
    case 7:
        xymove = {-1, -1};
        return xymove;
    case 8:
        xymove = {-1, 0};
        return xymove;
    case 9:
        xymove = {-1, 1};
        return xymove;
    }
}

bool DungeonCrawler::turn() {
    m_GUI->getMainWindow()->show();
    m_GUI->draw(*currentLevel);
    return true;
}

void DungeonCrawler::levelUp() {
    QTile::deleteAllQTiles();
    delete (*currentLevel);
    // currentLevel = m_levels.erase(currentLevel);
    // TODO refacor this
    (*currentLevel)->activateLevel();
    m_GUI->draw(*currentLevel);
    m_numberOfRemainingNPCs = (*currentLevel)->getNonPlayableCharacters().size();
}

void DungeonCrawler::moveCharacter(Character* character, Tile* tile, bool giveway) {
    if (giveway) {
        assert(character->getTexture() == 'A' && "A non attacker has asked to giveway");
        if (tile->getCharacter() && tile->getCharacter()->getTexture() != 'P') {
            Character* npc = tile->getCharacter();
            Tile* attackerTile = character->getTile();

            character->getTile()->setCharacter(nullptr);
            character->setTile(tile);
            tile->setCharacter(character);

            npc->setTile(attackerTile);
            attackerTile->setCharacter(npc);

            return;
        }
    }
    character->getTile()->setCharacter(nullptr);
    character->setTile(tile);
    tile->setCharacter(character);
}

Tile* DungeonCrawler::getEffectiveTile(Tile* currentTile, Tile* destinationTile) {
    // Player can't move if onLeave is false(ie: pit->floor) or onEnter is false (ie: trying to enter a wall)
    // Otherwise, if both are true, onEnter.first returns a pair: a bool indicating if a player might enter
    // and a pointer to itself or to the effective destination tile (a portal would return its sibling).

    if (currentTile->onLeave(destinationTile)) {
        std::pair<bool, Tile*> onEnterResult = destinationTile->onEnter();
        if (onEnterResult.first == false) {
            return currentTile;
        }
        return onEnterResult.second;
    }
    return currentTile;
}

void DungeonCrawler::holdFight(Character* attacker, Character* defender, Tile* disputedTile) {
    assert(attacker->isHuman() != defender->isHuman() && "Only Human VS NPC fights are allowed");
    Character* killer = nullptr;
    Character* victim = nullptr;
    defender->decrementFromHP(attacker->getStrength());
    if (defender->isAlive() == false) {
        killer = attacker;
        victim = defender;
    }
    else {
        attacker->decrementFromHP(defender->getStrength());
        if (attacker->isAlive() == false) {
            killer = defender;
            victim = attacker;
        }
    }

    if (killer) {
        m_GUI->playSound("qrc:/pics/textures/sounds/death.wav", 1);
        victim->getTile()->setCharacter(nullptr);
        moveCharacter(killer, disputedTile);
        assert(victim != nullptr);
        (*currentLevel)->removeCharacter(victim);
        delete victim;
        victim = nullptr;
    }
    else {
        m_GUI->playSound("qrc:/pics/textures/sounds/punch.wav", 1);
    }
    if (killer && killer->getTexture() == 'P') {
        m_numberOfRemainingNPCs -= 1;
        if (m_numberOfRemainingNPCs <= 0 && (*currentLevel) == m_lastLevel) {
            m_GUI->playSound("qrc:/pics/textures/sounds/win.ogg", 1);
            ;
            m_GUI->getMainWindow()->gameWon();
        }
    }
}
