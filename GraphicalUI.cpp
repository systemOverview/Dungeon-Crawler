#include "GraphicalUI.h"
#include <QAudioOutput>
#include <QMediaPlayer>
#include <QPainter>
#include "DungeonCrawler.h"
#include "MainWindow.h"
#include "startscreen.h"
#include "UiStrings.h"
#include <QTimer>
#include <QtCore/qpropertyanimation.h>
#include <QtWidgets/qapplication.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qmessagebox.h>


GraphicalUI::GraphicalUI(Level *lvl, DungeonCrawler *d)
{
    EventBus::subscribeToEvent<EventBus::AnimateTile>(this);
    EventBus::subscribeToEvent<EventBus::VisualizationStatus>(this);
    EventBus::subscribeToEvent<EventBus::DjikstraSearch>(this);

    level = lvl;
    dc = d;
    startScreen = new StartScreen(this);
    mainWindow = new MainWindow(lvl, this);
    m_visualizationLoop = new QEventLoop();
    m_graphMatrix = mainWindow->findChild<QGraphMatrix*> ("tileMatrixContainer");
    m_algorithmStepExplainerField = mainWindow->findChild<QTypeWriter*> ("algorithmStepExplainerField");

    // m_prevRegisterWidget = new QDjikstraPreviousRegister(mainWindow);
    if (m_graphMatrix==nullptr) assert(0 && "Graph Matrix Widget is not found");
    if (m_algorithmStepExplainerField==nullptr) assert (0 && "Algorithm Step Explainer Field is not found");

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
    QGameField* gameField = mainWindow->getGameField();
    Overlay* overlay = new Overlay(gameField, m_Qtiles);
    gameField->setChild(overlay);
    overlay->setGeometry(gameField->rect());
    overlay->show();

    QGridLayout *gameBoard = mainWindow->getGameBoard();
    std::vector<std::vector<Tile *> > *tiles = level->getTiles();
    for (int rowIterator = 0; rowIterator < level->getHeight(); rowIterator++) {
        for (int colIterator = 0; colIterator < level->getWidth(); colIterator++) {
            Tile *currentTile = (*tiles)[rowIterator][colIterator];
            QTile* tileWidget = new QTile(gameField, currentTile, gameBoard);
            m_Qtiles[{rowIterator, colIterator}] = tileWidget;
            gameBoard->addWidget(tileWidget, rowIterator, colIterator);
        }
    }
    overlay->setQTilesRegister(m_Qtiles);
    overlay->raise();

    moverlay = overlay;


}

void GraphicalUI::quitVisualizationLoop()
{
    mainWindow->centralWidget()->setLayout(nullptr);
    djikstraTest = false;
    QWidget* widget = new QWidget(mainWindow->centralWidget());
    m_isVisualizeModeOn = false;
    m_visualizationLoop->quit();
}


void GraphicalUI::onDjikstraSearch(DjikstraSearchEvent *event)
{

    assert (m_graphMatrix!=nullptr && "Graph matrix is not available. ");
    int counter = 0;
    DjikstaInitialSetup(event);
    for (auto& loop : event->getLoops()){
        DjikstraVisualizeLoop(event, loop, counter++);
    }


}

void GraphicalUI::DjikstaInitialSetup(DjikstraSearchEvent* event)
{
    m_algorithmStepExplainerField->setText(QString::fromStdString(DjikstraStrings::InitialSetup.at(DjikstraStrings::Intro)));
    m_graphMatrix->initializeMatrix(event->getStartingSearchRange(), "∞", 50);
    m_graphMatrix->setTextlessElementsStatusToBlocked();
    m_algorithmStepExplainerField->setText("");
    // Utilities::QtSleepMilliSeconds(1000);

    m_algorithmStepExplainerField->setText(QString::fromStdString(DjikstraStrings::InitialSetup.at(DjikstraStrings::SetStartingToZero)));
    std::pair<int,int> startingTileCords = event->getStartingTileCords();
    m_graphMatrix->setElementText(startingTileCords, {"0"});
    m_graphMatrix->setElementState(startingTileCords, QGraphMatrix::DjikstraState::Calculated);
}

void GraphicalUI::DjikstraVisualizeLoop(DjikstraSearchEvent* event, DjikstraSearchEvent::Loop loop, int loopId)
{
    QTile* extractedQTile = m_Qtiles.at(loop.getExtractedTileCords());
    m_graphMatrix->setElementState(loop.getExtractedTileCords(), QGraphMatrix::DjikstraState::Calculated);
    Utilities::QtSleepMilliSeconds(1000);
    if (loopId==0){
        m_algorithmStepExplainerField->setText(QString::fromStdString(DjikstraStrings::Explainers.at(DjikstraStrings::RemoveStartingFromQueue)));
        m_algorithmStepExplainerField->setText(QString::fromStdString(DjikstraStrings::Explainers.at(DjikstraStrings::ExplainDjikstraValues)));
    }
    for (DjikstraSearchEvent::Loop::Neighbour& neighbour : loop.getNeighbourTiles()){
        QTile* neighbourQTile = m_Qtiles.at(neighbour.getCords());
        moverlay->addArrowFromCords(loop.getExtractedTileCords(), neighbour.getCords(), loopId, loopId);
        if (neighbour.wasDjikstraValueUpdated()){
            m_graphMatrix->setElementText(neighbour.getCords(), Utilities::FloatToString(neighbour.getDjikstraValue(), 1));
            m_graphMatrix->visualizeElement(neighbour.getCords(), 200);
        }
        else{
            Utilities::QtSleepMilliSeconds(200);
        }
    }


    Utilities::QtSleepMilliSeconds(1000);
    moverlay->removeArrowsByGroupId(loopId);

}

std::pair<int, int> GraphicalUI::move()
{
    return lastMove;
}

void GraphicalUI::move(std::pair<int, int> xymove)
{
    mainWindow->findChild<QGraphMatrix*> ("tileMatrixContainer")->resetMatrix();
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
    default:
        throw std::invalid_argument("Invalid or unhandled step identifier supplied to the translation function. ");

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
