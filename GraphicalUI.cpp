#include "GraphicalUI.h"
#include "startscreen.h"
#include "MainWindow.h"
#include "DungeonCrawler.h"
#include <sstream>
#include <QPainter>
#include <QMediaPlayer>
#include <QAudioOutput>
GraphicalUI::GraphicalUI(Level *lvl, DungeonCrawler *d) {
    level = lvl;
    dc = d;
    startScreen = new StartScreen(this);
    mainWindow = new MainWindow(lvl, this);
}

GraphicalUI::~GraphicalUI() {
    delete startScreen;
    delete mainWindow;
}

void GraphicalUI::switchWindow() {
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

void GraphicalUI::playSound(QString soundLink, float volume)
{
    QMediaPlayer* player = new QMediaPlayer;
    QAudioOutput* audioOutput = new QAudioOutput;
    player->setAudioOutput(audioOutput);
    player->setSource(QUrl(soundLink));
    audioOutput->setVolume(volume);
    player->play();

}

void GraphicalUI::draw(Level *level) {

    removeHealthBars();
    QGridLayout *gameBoard = mainWindow->getGameBoard();
    std::vector<std::vector<Tile *> > *tiles = level->getTiles();
    for (int rowIterator=0; rowIterator<level->getHeight(); rowIterator++){
        for (int colIterator=0;colIterator<level->getWidth();colIterator++){
            Tile* currentTile = (*tiles)[rowIterator][colIterator];
            QLayoutItem* itemAtTilePosition = gameBoard->itemAtPosition(rowIterator, colIterator);
            if (!itemAtTilePosition){
                QWidget *tileWidget = new QWidget(nullptr);
                tileWidget->setMinimumSize(50, 50);
                tileWidget->setFixedSize(50,50);
                std::ostringstream style;
                std::ostringstream bStyle;
                style << "border-image : url(" << currentTile->getTexturePath() << ") 0 0 0 0 stretch stretch;";
                bStyle << "background-image:url(" << currentTile->getTexturePath() << "); background-repeat: no-repeat; background-position:center;";

                QString bgStyleFull = QString::fromStdString(style.str());
                tileWidget->setStyleSheet(bgStyleFull);

                gameBoard->addWidget(tileWidget, rowIterator, colIterator);
            }
            else{
                QList<QWidget*> children = itemAtTilePosition->widget()->findChildren<QWidget*>(QString(),Qt::FindDirectChildrenOnly);
                for (auto it= children.begin();it!=children.end();it++){
                    delete (*it);
                }
            }

            if (currentTile->hasCharacter()){
                QWidget* parentWidget = gameBoard->itemAtPosition(rowIterator, colIterator)->widget();
                Character* characterAtTile = currentTile->getCharacter();
                QWidget* characterWidget = new QWidget(parentWidget);
                characterWidget->setFixedSize(50,40);
                characterWidget->move(0,10);
                std::ostringstream style;
                std::ostringstream bStyle;
                style << "border-image : url(:" << characterAtTile->getTexturePath() <<
                    ") 20 0 0 0 stretch stretch;";

                float healthPercentage = (static_cast<float>(currentTile->getCharacter()->getCurrentHP())/static_cast<float>(currentTile->getCharacter()->getMaxHP()))*100;
                QWidget* healthBar = generateHealthBar(healthPercentage, nullptr);
                healthBar->setFixedSize(50,10);
                characterWidget->show();
                // healthBar->show();
                QString bgStyleFull = QString::fromStdString(style.str());
                characterWidget->setStyleSheet(bgStyleFull);
                addHealthBar(healthBar);
                gameBoard->addWidget(healthBar, rowIterator, colIterator, Qt::AlignTop);

            }

        }
    }

}

QWidget* GraphicalUI::generateHealthBar(int percentage, QWidget* parent)
{
    QWidget* healthBar = new QWidget(parent);
    float stopRedAt = std::clamp(percentage / 100.0f, 0.0f, 1.0f);
    if (stopRedAt<0){
        stopRedAt=0;
        percentage=0;
    }
    float startWhiteFrom = stopRedAt+0.01;
    if (stopRedAt>=1){
        stopRedAt=0.99;
        startWhiteFrom = 1;
    }
    std::ostringstream style;
    std::ostringstream bStyle;
    style << "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
          << "stop:0 red, stop:" << stopRedAt << " red, "
          << "stop:" << startWhiteFrom << " white, stop:1 white);"
          << "border:1px solid black;";

    healthBar->setStyleSheet(QString::fromStdString(style.str()));

    return healthBar;
}

std::pair<int, int> GraphicalUI::move()
{
    return lastMove;
}



int GraphicalUI::move(std::pair<int, int> xymove) {
    lastMove = xymove;
    dc->move();
}

std::pair<int, int> GraphicalUI::translateMove(int step) {
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
            xymove = {1, 1}; /*1-3, 2,2*/
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

void GraphicalUI::start() {
    mainWindow->show();
}

std::pair<int, int> GraphicalUI::getLastMove() {
    return lastMove;
}

void GraphicalUI::addHealthBar(QWidget* healthBar)
{
    m_healthBars.push_back(healthBar);
}

void GraphicalUI::removeHealthBars()
{
    for (auto it = m_healthBars.begin(); it != m_healthBars.end(); )
    {
        delete *it;
        it = m_healthBars.erase(it); // erase returns the next valid iterator
    }
}



QDialog *GraphicalUI::getStartScreen() {
    return startScreen;
}

QMainWindow *GraphicalUI::getMainWindow() {
    return mainWindow;
}
