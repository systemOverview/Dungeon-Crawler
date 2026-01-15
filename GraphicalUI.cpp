#include "GraphicalUI.h"
#include <QAudioOutput>
#include <QMediaPlayer>
#include <QPainter>
#include "DungeonCrawler.h"
#include "MainWindow.h"
#include "startscreen.h"
#include "QHealthBar.h"
#include "QCharacter.h"
#include <sstream>
GraphicalUI::GraphicalUI(Level *lvl, DungeonCrawler *d)
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
    if (currentWindow == 0) {
        startScreen->hide();
        mainWindow->show();
        currentWindow = 1;
    } else {
        mainWindow->hide();
        startScreen->show();
        startScreen->exec();
        currentWindow = 0;
    }
}

void GraphicalUI::deleteAllTiles()
{
    QGridLayout *gameBoard = mainWindow->getGameBoard();
    for (int rowIterator = 0; rowIterator < level->getHeight(); rowIterator++) {
        for (int colIterator = 0; colIterator < level->getWidth(); colIterator++) {
            QLayoutItem *itemAtTilePosition = gameBoard->itemAtPosition(rowIterator, colIterator);
            if (itemAtTilePosition->widget()){
                QWidget* widgetAtTilePosition = itemAtTilePosition->widget();
                gameBoard->removeWidget(widgetAtTilePosition);
                delete widgetAtTilePosition;
            }
        }
    }

}

void GraphicalUI::playSound(QString soundLink, float volume)
{
    QMediaPlayer *player = new QMediaPlayer;
    QAudioOutput *audioOutput = new QAudioOutput;
    player->setAudioOutput(audioOutput);
    player->setSource(QUrl(soundLink));
    audioOutput->setVolume(volume);
    player->play();
}

void GraphicalUI::draw(Level *level){
    QGridLayout *gameBoard = mainWindow->getGameBoard();
    std::vector<std::vector<Tile *> > *tiles = level->getTiles();
    for (int rowIterator = 0; rowIterator < level->getHeight(); rowIterator++) {
        for (int colIterator = 0; colIterator < level->getWidth(); colIterator++) {
            Tile *currentTile = (*tiles)[rowIterator][colIterator];
            QTile* tileWidget = new QTile(nullptr, currentTile, gameBoard);
            gameBoard->addWidget(tileWidget, rowIterator, colIterator);
            if (tileWidget->getCharacter()){
                gameBoard->addWidget(tileWidget->getCharacter()->getHealthBar(), rowIterator, colIterator, Qt::AlignTop);
            }
        }
    }

}

std::pair<int, int> GraphicalUI::move()
{
    return lastMove;
}

void GraphicalUI::move(std::pair<int, int> xymove)
{
    lastMove = xymove;
    dc->move();
}

std::pair<int, int> GraphicalUI::translateMove(int step)
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

void GraphicalUI::start()
{
    mainWindow->show();
}

std::pair<int, int> GraphicalUI::getLastMove()
{
    return lastMove;
}

void GraphicalUI::addHealthBar(QWidget *healthBar)
{
    m_healthBars.push_back(healthBar);
}

void GraphicalUI::removeHealthBars()
{
    for (auto it = m_healthBars.begin(); it != m_healthBars.end();) {
        (*it)->setParent(nullptr);
        it = m_healthBars.erase(it);
    };
}

QDialog *GraphicalUI::getStartScreen()
{
    return startScreen;
}

QMainWindow *GraphicalUI::getMainWindow()
{
    return mainWindow;
}
