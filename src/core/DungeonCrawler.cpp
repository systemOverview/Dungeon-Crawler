//
// Created by MBK on 13.11.25.
//

#include "DungeonCrawler.h"
#include <QApplication>
#include <QWindow>
#include <QtCore/qtimer.h>
#include "list.cpp"
#include "startscreen.h"

DungeonCrawler::DungeonCrawler()
{

    m_startScreen = new StartScreen(this);
    m_startScreen->show();
}
void DungeonCrawler::buildGame(GameSourceOption option)
{
    if (option==GameSourceOption::FromStrings){
        for (std::string& gameString : gameStrings){
            Level* lvl = new Level(10, 10, gameString, true);
            levels.push_back(lvl);
            m_lastLevel = lvl;
        }
    }
    else{
        std::ifstream file(GameData::DataFilePath);

        json parsedJson = json::parse(file);
        for (const auto& level : parsedJson["Levels"]){
            Level* lvl = new Level(level);
            levels.push_back(lvl);
            m_lastLevel = lvl;


        }
    }

    m_startScreen->hide();



    currentLevel = levels.begin();
    (*currentLevel)->activateLevel();
    GUI = new GraphicalUI(*(currentLevel), this);
    GUI->getMainWindow()->show();
    GUI->draw(*currentLevel);
    m_numberOfRemainingNPCs = (*currentLevel)->getNonPlayableCharacters().size();
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
    Character* humanCharacter = (*currentLevel)->getPlayableCharacter();
    Tile* currentTile = humanCharacter->getTile();
    int newRow = currentTile->getRow() + moveToPerform.first;
    int newColumn = currentTile->getColumn() + moveToPerform.second;
    Tile* wantedTile = (*currentLevel)->getTile(newRow, newColumn);
    Tile* effectiveTile = getEffectiveTile(currentTile, wantedTile);
    if (effectiveTile->getTexture()=='$'){
        levelUp();
        return ;
    }
    if (effectiveTile->getTexture()=='!'){
        GUI->playSound("qrc:/pics/textures/sounds/win.ogg", 1);
        GUI->getMainWindow()->gameWon();
        return;
    }
    if (effectiveTile == currentTile);
    else if  (effectiveTile->hasCharacter() && effectiveTile->getCharacter()->isHuman()==false){
        holdFight(humanCharacter, effectiveTile->getCharacter(), effectiveTile);
    }
    else{
        moveCharacter(humanCharacter, effectiveTile);
    }
    Utilities::QtSleepMilliSeconds(100);

    std::vector<Character *> NPCs = (*currentLevel)->getNonPlayableCharacters();
    for (auto &NPC : NPCs) {
        if (NPC->isAlive()) {
            // int move = NPC->getController()->move(currentLevel);
            std::pair<int, int> moveToPerform = NPC->move();
            Tile* currentTile = NPC->getTile();
            int newRow = currentTile->getRow() + moveToPerform.first;
            int newColumn = currentTile->getColumn() + moveToPerform.second;
            Tile* wantedTile = (*currentLevel)->getTile(newRow, newColumn);
            Tile* effectiveTile = getEffectiveTile(currentTile, wantedTile);
            if (effectiveTile == currentTile);
            else if  (effectiveTile->hasCharacter() && effectiveTile->getCharacter()->isHuman()==true){
                holdFight(NPC, effectiveTile->getCharacter(), effectiveTile);
            }
            else{
                if (effectiveTile->hasCharacter() && effectiveTile->getCharacter()->isHuman()==false && NPC->getTexture()!='A'){return;}
                //if an NPC wants to access a tile that already has another NPC, it respects its teammate and stays where it is.
                if (NPC->getTexture()=='A'){moveCharacter(NPC, effectiveTile, true);}
                else {moveCharacter(NPC, effectiveTile, false);}
            }
        }
    }

    JsonGenerator::saveGameState(levels);

}

void DungeonCrawler::levelUp()
{
    QTile::deleteAllQTiles();
    delete (*currentLevel);
    currentLevel = levels.erase(currentLevel);
    // TODO refacor this
    (*currentLevel)->activateLevel();
    GUI->draw(*currentLevel);
    m_numberOfRemainingNPCs = (*currentLevel)->getNonPlayableCharacters().size();


}

void DungeonCrawler::moveCharacter(Character *character, Tile *tile, bool giveway)
{
    qDebug() << character->getTexture() << " wants to move to " << tile->getCordsAsPair();
    if (giveway){
        assert(character->getTexture()=='A' && "A non attacker has asked to giveway");
        if (tile->getCharacter() && tile->getCharacter()->getTexture()!='P'){
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

Tile* DungeonCrawler::getEffectiveTile(Tile* currentTile, Tile *destinationTile)
{
    // Player can't move if onLeave is false(ie: pit->floor) or onEnter is false (ie: trying to enter a wall)
    // Otherwise, if both are true, onEnter.first returns a pair: a bool indicating if a player might enter
    // and a pointer to itself or to the effective destination tile (a portal would return its sibling).

    if (currentTile->onLeave(destinationTile)){
        std::pair<bool, Tile *> onEnterResult = destinationTile->onEnter();
        if (onEnterResult.first == false) {
            return currentTile;
        }
        return onEnterResult.second;
    }
    return currentTile;

}

void DungeonCrawler::holdFight(Character* attacker, Character* defender, Tile *disputedTile)
{
    assert (attacker->isHuman()!=defender->isHuman() && "Only Human VS NPC fights are allowed");
    Character* killer = nullptr;
    Character* victim = nullptr;
    defender->decrementFromHP(attacker->getStrength());
    if (defender->isAlive()==false){
        killer = attacker;
        victim = defender;
    }
    else{
        attacker->decrementFromHP(defender->getStrength());
        if (attacker->isAlive()==false){
            killer = defender;
            victim = attacker;
        }
    }

    if (killer){
        GUI->playSound("qrc:/pics/textures/sounds/death.wav", 1);
        victim->getTile()->setCharacter(nullptr);
        moveCharacter(killer, disputedTile);
        assert (victim!=nullptr );
        (*currentLevel)->removeCharacter(victim);
        delete victim;
        victim = nullptr;

    }
    else{
        GUI->playSound("qrc:/pics/textures/sounds/punch.wav", 1);
    }
    if (killer && killer->getTexture()=='P'){
        m_numberOfRemainingNPCs -=1;
        if (m_numberOfRemainingNPCs<=0 && (*currentLevel) == m_lastLevel){
            GUI->playSound("qrc:/pics/textures/sounds/win.ogg", 1);
            ;GUI->getMainWindow()->gameWon();}
    }


}


