#include "GraphicalUI.h"
#include "startscreen.h"
#include "MainWindow.h"
#include "DungeonCrawler.h"
#include <sstream>


GraphicalUI::GraphicalUI(Level *lvl, DungeonCrawler* d)
{
    level = lvl;
    dc = d;
    startScreen = new StartScreen(this);
    mainWindow = new MainWindow(lvl, this);



}
GraphicalUI::~GraphicalUI()
{
    delete startScreen;
    delete mainWindow;
}

void GraphicalUI::switchWindow()
{
    if (currentWindow == 0){
        startScreen->hide();
        mainWindow->show();
        currentWindow = 1;
    }
    else{
        mainWindow->hide();
        startScreen->show();
        startScreen->exec();
        currentWindow = 0;

    }
}



void GraphicalUI::draw(Level *level)
{
    QGridLayout* gameBoard = mainWindow->getGameBoard();
    std::vector <std::vector<Tile*>>* tiles = level->getTiles();
    for (int i=0;i<level->getHeight();i++) {
        for (int j=0; j<level->getWidth();j++) {
            Tile* currentTile = (*tiles)[i][j];
            QWidget* tileWidget = new QWidget();
            tileWidget->setMinimumSize(50,50);
            gameBoard->addWidget(nullptr, i,j);

            if (currentTile==nullptr) {
            }

            else {
                // cast into its type
                if (currentTile->hasCharacter()) {
                    // #centralwidget{
                    //                     border-image : url(:/pics/bloody_frame.png) 0 0 0 0 stretch stretch;
                    //                 }
                    std::ostringstream style ;
                    style << "border-image : url(:" << "/pics/ textures/char/front/char_front_1.png" << ") 0 0 0 0 stretch stretch;";

                    QString bgStyleFull = QString::fromStdString(style.str());
                    tileWidget->setStyleSheet(bgStyleFull);


                }
                else {
                    std::ostringstream style ;
                    style << "border-image : url(" << currentTile->getTexturePath() << ") 0 0 0 0 stretch stretch;";

                    QString bgStyleFull = QString::fromStdString(style.str());
                    tileWidget->setStyleSheet(bgStyleFull);

                }
            }

            gameBoard->addWidget(tileWidget, i,j);
        }
    }
}

int GraphicalUI::move()
{
    return 0;
}

int GraphicalUI::move(std::pair<int,int> xymove) {

    dc->move(xymove);
}

std::pair<int, int> GraphicalUI::translateMove(int step) {
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

void GraphicalUI::start()
{
    mainWindow->show();
}



QDialog *GraphicalUI::getStartScreen()
{
    return startScreen;
}

QMainWindow *GraphicalUI::getMainWindow()
{
    return mainWindow;
}

