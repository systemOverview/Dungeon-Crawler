//
// Created by MBK on 13.11.25.
//

#include "DungeonCrawler.h"
#include <QApplication>
#include <QWindow>
#include "list.cpp"

DungeonCrawler::DungeonCrawler()
{
    m_humanCharacter = new Character("P",
                                              "/pics/textures/char/front/char_front_1.png",
                                              20,
                                              20);
    for (auto& gameString : gameStrings){
        Level *lvl = new Level(10, 10, gameString);
        levels.push_back(lvl);
    }
    currentLevel = levels.begin();
    (*currentLevel)->placePlayingCharacter(m_humanCharacter);
    GUI = new GraphicalUI(*(currentLevel), this);
    m_humanCharacter->setController(GUI);
    // GUI->getStartScreen()->show();
    GUI->getMainWindow()->show();
    GUI->draw(*currentLevel);
}

std::pair<int, int> DungeonCrawler::translateMove(int step) const
{
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

bool DungeonCrawler::turn()
{
    GUI->getMainWindow()->show();
    GUI->draw(*currentLevel);
    return true;
}

void DungeonCrawler::move()
{
    std::pair<int, int> moveToPerform = GUI->getLastMove();
    Character *humanCharacter = (*currentLevel)->getPlayableCharacter();
    Tile *currentTile = humanCharacter->getTile();
    int newRow = currentTile->getRow() + moveToPerform.first;
    int newColumn = currentTile->getColumn() + moveToPerform.second;
    Tile *wantedTile = (*currentLevel)->getTile(newRow, newColumn);
    bool isMoveAllowed = currentTile->moveTo(wantedTile, humanCharacter);
    if (isMoveAllowed && wantedTile->getTexture()=="$"){
        levelUp();
    }
    else{
        std::vector<Character *> NPCs = (*currentLevel)->getNonPlayableCharacters();
        for (auto &NPC : NPCs) {
            if (NPC->isAlive()) {
                // int move = NPC->getController()->move(currentLevel);
                std::pair<int, int> moveToPerform = NPC->move();
                Tile *currentTile = NPC->getTile();
                int newRow = currentTile->getRow() + moveToPerform.first;
                int newColumn = currentTile->getColumn() + moveToPerform.second;
                Tile *wantedTile = (*currentLevel)->getTile(newRow, newColumn);
                bool isMoveAllowed = currentTile->moveTo(wantedTile, NPC);
            }
        }

    }
    // turn();
}

void DungeonCrawler::levelUp()
{
    GUI->deleteAllTiles();
    currentLevel = ++currentLevel;
    // TODO refacor this
    // m_humanCharacter->removeObserver(m_humanCharacter->getQChatacter());
    (*currentLevel)->placePlayingCharacter(m_humanCharacter);
    m_humanCharacter->setController(GUI);
    GUI->draw(*currentLevel);

}
