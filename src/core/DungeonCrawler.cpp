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

DungeonCrawler::DungeonCrawler() {
    assert(SINGLETON_INSTANCE == nullptr && "Multiple instanced of DC are being created");
    m_GUI = new GraphicalUI();
    SINGLETON_INSTANCE = this;
    connect(m_GUI, &GraphicalUI::gameStarted, this, &DungeonCrawler::buildGame);
}

void DungeonCrawler::buildGame() {
    Level* level = new Level(gameStrings[0]);
    m_levels.push_back(level);
    m_GUI->createLevelUI(level->getTiles());
}

void DungeonCrawler::ProcessNewCharacter(Character* character) {
    AbstractController* controller = character->getController();
    connect(SINGLETON_INSTANCE,
            &DungeonCrawler::move,
            controller,
            &AbstractController::initiateMove);
}

bool DungeonCrawler::ValidateMove(Tile* from, Tile* to) {
    Coordinates fromCords = from->getCoordinates();
    Coordinates toCords = to->getCoordinates();
}

// ValidateMove helpers

bool DungeonCrawler::IsTileInNeighbouringRange(std::pair<int, int> fromCords,
                                               std::pair<int, int> toCords) {
    // if (fromCords.) }

    // old
}

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
