#include "GraphicalUI.h"
#include <QAudioOutput>
#include <QMediaPlayer>
#include <QPainter>
#include "DungeonCrawler.h"
#include "MainWindow.h"
#include "startscreen.h"
#include <QTimer>


GraphicalUI::GraphicalUI(Level *lvl, DungeonCrawler *d)
{
    EventBus::subscribeToEvent<EventBus::AnimateTile>(this);
    EventBus::subscribeToEvent<EventBus::VisualizationStatus>(this);

    level = lvl;
    dc = d;
    startScreen = new StartScreen(this);
    mainWindow = new MainWindow(lvl, this);
    m_visualizationLoop = new QEventLoop();
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
    if (level!=this->level){
        this->level = level;
    }
    QGridLayout *gameBoard = mainWindow->getGameBoard();
    std::vector<std::vector<Tile *> > *tiles = level->getTiles();
    for (int rowIterator = 0; rowIterator < level->getHeight(); rowIterator++) {
        for (int colIterator = 0; colIterator < level->getWidth(); colIterator++) {
            Tile *currentTile = (*tiles)[rowIterator][colIterator];
            QTile* tileWidget = new QTile(nullptr, currentTile, gameBoard);

            m_Qtiles[{rowIterator, colIterator}] = tileWidget;
            gameBoard->addWidget(tileWidget, rowIterator, colIterator);
        }
    }


}

void GraphicalUI::quitVisualizationLoop()
{
    m_isVisualizeModeOn = false;
    m_visualizationLoop->quit();
}

void GraphicalUI::onAnimateTile(AnimateTileEvent* event) {
    qDebug() << event;
    m_animationsQueue.emplace(event);
}

void GraphicalUI::onVisualizationChange(VisualizationStatusEvent* eventt)
{
    if (eventt->getStatus()==VisualizationStatusEvent::Quit){
        return;
    }
    while (m_isVisualizeModeOn && !m_animationsQueue.empty()){
        AnimateTileEvent* event = m_animationsQueue.front();
        QGridLayout *gameBoard = mainWindow->getGameBoard();
        std::string overlayText (event->getOverlayText());
        Tile* tile = event->getAffectedTile();
        QTile* tileToVisualize = m_Qtiles[{tile->getRow(),tile->getColumn()}];
        auto v = event->getVisualizations();
        for (auto it = v.begin(); it!=v.end(); it++){
            switch (*it){
            case AnimateTileEvent::colorizeTile :{tileToVisualize->colorize();break;
            }

            case AnimateTileEvent::overlayText :{
                if (overlayText!=tileToVisualize->getTextOverlay()){
                    tileToVisualize->setTextOverlay(overlayText);
                }
                break;
            }

            }
        }
        QTimer::singleShot(m_loopDuration, m_visualizationLoop, &QEventLoop::quit);
        m_visualizationLoop->exec();
        qDebug() << m_loopDuration;
        m_animationsQueue.pop();
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
