#include "GraphicalUI.h"
#include <QAudioOutput>
#include <QMediaPlayer>
#include <QPainter>
#include "DungeonCrawler.h"
#include "MainWindow.h"
#include "startscreen.h"
#include "Constants.h"
#include <QTimer>
#include <QtCore/qpropertyanimation.h>
#include <QtWidgets/qapplication.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qmessagebox.h>
#include <QtWidgets/qpushbutton.h>

void GraphicalUI::setVisualizationMode(VisualizationMode mode)
{
    m_visualizationMode = mode;
}

GraphicalUI::GraphicalUI(Level *lvl, DungeonCrawler *d)
{
    EventBus::subscribeToEvent<EventBus::AnimateTile>(this);
    EventBus::subscribeToEvent<EventBus::VisualizationStatus>(this);
    EventBus::subscribeToEvent<EventBus::DjikstraSearch>(this);
    EventBus::subscribeToEvent<EventBus::CharacterHealthChange>(this, lvl->getPlayableCharacter());

    level = lvl;
    dc = d;
    mainWindow = new MainWindow(lvl, this);
    m_graphMatrix = mainWindow->findChild<QGraphMatrix*> ("tileMatrixContainer");
    m_algorithmStepExplainerField = mainWindow->findChild<QTypeWriter*> ("algorithmStepExplainerField");

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
    if (m_overlayWidget){
        delete m_overlayWidget;
        // draw is only called once per level, if it was called again that means that the previous level was finished
        // This is also necessary so it doesn't keep trying to show arrows of deleted QTiles.

    }
    QGameField* gameField = mainWindow->getGameField();
    m_overlayWidget  = new QOverlay(gameField, m_Qtiles);

    gameField->setChild(m_overlayWidget );
    m_overlayWidget ->setGeometry(gameField->rect());
    m_overlayWidget ->show();
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
      m_overlayWidget ->setQTilesRegister(m_Qtiles);
      m_overlayWidget ->raise();

}

void GraphicalUI::onDjikstraSearch(DjikstraSearchEvent *event)
{

    m_overlayWidget->removeAllArrows();
    if (m_visualizationMode==None){return;}
    if (m_visualizationMode == OnlyFinalPath){
        for (auto loop: event->getLoops()){
            LevelGraph graph;
            if (loop.getExtractedTileCords()==event->getTargetTileCords()){
                auto previousRegister = loop.getPreviousRegister();
                QTile* extractedQTile = m_Qtiles.at(loop.getExtractedTileCords());
                std::vector<std::pair<int,int>> path = {graph.generatePathFromPreviousRegister(previousRegister, loop.getExtractedTileCords(), LevelGraph::PathDirection::FromStartingToTarget, LevelGraph::PathCoordinateSystem::Absolute)};
                m_overlayWidget->addArrowPathBetweenMultipleTiles(path, true);
            }
        }
        return;
    }
    mainWindow->getArrowField()->setVisible(false);
    assert (m_graphMatrix!=nullptr && "Graph matrix is not available. ");
    int counter = 0;
    DjikstaInitialSetup(event);
    for (auto& loop : event->getLoops()){
        if (m_isVisualizeModeOn==false){break;}
        DjikstraVisualizeLoop(event, loop, counter++);
    }
    m_overlayWidget->highlightArrowPathAndRemoveOthers(event->getTargetTileCords(), event->getStartingTileCords());
    // m_overlayWidget->removeAllArrows();
    mainWindow->getArrowField()->setVisible(true);
    // setNoTextDjikstraMode(true);
    m_graphMatrix->resetMatrix();
    m_algorithmStepExplainerField->setText("");

}

void GraphicalUI::onCharacterHealthChange(CharacterHealthChangeEvent* event)
{
    // assert (event->getCharacter() == level->getPlayableCharacter() && "Wrong character");
    if (event->getCharacter()->isAlive()==false){
        QApplication::processEvents();
        qDebug() << "received";
        playSound("qrc:/pics/textures/sounds/gameover.mp3", 1);

        mainWindow->gameOver();
    }

}

void GraphicalUI::DjikstaInitialSetup(DjikstraSearchEvent* event)
{
    if (m_visualizationMode==FullVisualization){m_algorithmStepExplainerField->setText(QString::fromStdString(DjikstraStrings::InitialSetup.at(DjikstraStrings::Intro)));}
    m_graphMatrix->initializeMatrix(event->getStartingSearchRange(), "∞", 50);
    m_graphMatrix->setTextlessElementsStatusToBlocked();
    m_algorithmStepExplainerField->setText("");

     if (m_visualizationMode==FullVisualization) m_algorithmStepExplainerField->setText(QString::fromStdString(DjikstraStrings::InitialSetup.at(DjikstraStrings::SetStartingToZero)));
    std::pair<int,int> startingTileCords = event->getStartingTileCords();
    m_graphMatrix->setElementText(startingTileCords, {"0"});
    m_graphMatrix->setElementState(startingTileCords, QGraphMatrix::DjikstraState::Calculated);
     if (m_visualizationMode==FullVisualization) m_algorithmStepExplainerField->setText(QString::fromStdString(DjikstraStrings::InitialSetup.at(DjikstraStrings::CreateQueue)));

}

void GraphicalUI::DjikstraVisualizeLoop(DjikstraSearchEvent* event, DjikstraSearchEvent::Loop loop, int loopId)
{
    LevelGraph graph;
    auto previousRegister = loop.getPreviousRegister();
    QTile* extractedQTile = m_Qtiles.at(loop.getExtractedTileCords());
    m_graphMatrix->setElementState(loop.getExtractedTileCords(), QGraphMatrix::DjikstraState::Calculated);
    std::vector<std::pair<int,int>> path = {graph.generatePathFromPreviousRegister(previousRegister, loop.getExtractedTileCords(), LevelGraph::PathDirection::FromStartingToTarget, LevelGraph::PathCoordinateSystem::Absolute)};
    m_overlayWidget->addArrowPathBetweenMultipleTiles(path);
    Utilities::QtSleepMilliSeconds(100);
    if (loopId==0){
         if (m_visualizationMode==FullVisualization) m_algorithmStepExplainerField->setText(QString::fromStdString(DjikstraStrings::Explainers.at(DjikstraStrings::RemoveStartingFromQueue)));
        Utilities::QtSleepMilliSeconds(QtVisualiation::SleepTimeAfterText);
         if (m_visualizationMode==FullVisualization) m_algorithmStepExplainerField->setText(QString::fromStdString(DjikstraStrings::Explainers.at(DjikstraStrings::ExplainDjikstraValues)));
        Utilities::QtSleepMilliSeconds(QtVisualiation::SleepTimeAfterText);
    }

    for (DjikstraSearchEvent::Loop::Neighbour& neighbour : loop.getNeighbourTiles()){
        QTile* neighbourQTile = m_Qtiles.at(neighbour.getCords());
        m_overlayWidget->addEdge(loop.getExtractedTileCords(), neighbour.getCords(), loopId, loopId);
        if (neighbour.wasDjikstraValueUpdated()){
            m_graphMatrix->setElementText(neighbour.getCords(), Utilities::FloatToString(neighbour.getDjikstraValue(), 1));
            m_graphMatrix->visualizeElement(neighbour.getCords(), QtVisualiation::SleepTimeBetweenArrows);
        }
        else{
            Utilities::QtSleepMilliSeconds(QtVisualiation::SleepTimeBetweenArrows);
        }
    }


    Utilities::QtSleepMilliSeconds(100);
    m_overlayWidget->removeArrowsByGroupId(loopId);
    m_algorithmStepExplainerField->setText("");

}

void GraphicalUI::saveGame()
{
    mainWindow->showTerminal();
    JsonGenerator::saveGameState(dc->levels);
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

MainWindow *GraphicalUI::getMainWindow()
{
    return mainWindow;
}
