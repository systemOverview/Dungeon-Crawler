//
// Created by MBK on 13.11.25.
//

#include "DungeonCrawler.h"
#include "list.cpp"



DungeonCrawler::DungeonCrawler() {
    Level* lvl = new Level(10,10, gameStrings[0]);
    levels.push_back(lvl);
    currentLevel = lvl;
    GUI = new GraphicalUI(currentLevel, this);
    Character* s = lvl->getPlayableCharacter();
    s->setController(GUI);
    qDebug()<<s;
    turn();

}

std::pair<int,int> DungeonCrawler::translateMove(int step) const {
    std::pair<int,int> xymove;


    switch (step) {
        /* to go down, you add +1 to the row, to go up -1 the row, right +1 the column, left -1 the column*/
        case 1:
            xymove ={1,-1};
            return xymove;
        case 2:
            xymove = {1,0};
            return xymove;
        case 3:
            xymove = {1,1}; /*1-3, 2,2*/
            return xymove;
        case 4:
            xymove = {0,-1};
            return xymove;
        case 5:
            xymove = {0,0};
            return xymove;
        case 6:
            xymove = {0,1};
            return xymove;
        case 7:
            xymove = {-1,-1};
            return xymove;
        case 8:
            xymove = {-1,0};
            return xymove;
        case 9:
            xymove = {-1,1};
            return xymove;

    }
}

bool DungeonCrawler::turn() {

    GUI->getMainWindow()->show();

    GUI->draw(currentLevel);
    // move();
    return true;
}

void DungeonCrawler::move(std::pair <int,int> xymove) {
    currentLevel->getPlayableCharacter()->move(currentLevel, GUI->getLastMove());
    // Character* character = currentLevel->getPlayableCharacter();

    // Tile* currentTile = character->getTile();
    // // int move = character->move(currentLevel);
    // // std::pair <int,int> xymove = translateMove(move);

    // if (xymove.first ==0 && xymove.second == 0) {
    //     return; /*move number 5 does nothing*/
    // }
    // int newRow = currentTile->getRow()+xymove.first;
    // int newColumn = currentTile->getColumn()+xymove.second;
    // Tile* wantedTile = currentLevel->getTile(newRow, newColumn);
    // bool isMoveAllowed = currentTile->moveTo(wantedTile,character);

    // std::vector<Character*> NPCs = currentLevel->getNonPlayableCharacters();
    // for (auto & NPC : NPCs) {
    //     int move = NPC->getController()->move(currentLevel);
    // }



    GUI->draw(currentLevel);



}
