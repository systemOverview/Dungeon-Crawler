#include "GraphicalUI.h"
#include <QAudioOutput>
#include <QMediaPlayer>
#include <QPainter>
#include <QShortcut>
#include <QTimer>
#include <QtCore/qpropertyanimation.h>
#include <QtCore/qsequentialanimationgroup.h>
#include <QtStateMachine/qfinalstate.h>
#include <QtWidgets/qapplication.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qmessagebox.h>
#include <QtWidgets/qpushbutton.h>
#include "CharacterTile_UI_PlacementMediator.h"
#include "Constants.h"
#include "DungeonCrawler.h"
#include "MainWindow.h"
#include "TileItem.h"
#include "startscreen.h"

GraphicalUI::GraphicalUI() {
    // EventBus::subscribeToEvent<EventBus::AnimateTile>(this);
    // EventBus::subscribeToEvent<EventBus::VisualizationStatus>(this);
    // EventBus::subscribeToEvent<EventBus::DjikstraSearch>(this);
    // EventBus::subscribeToEvent<EventBus::CharacterHealthChange>(this, lvl->getPlayableCharacter());
    m_mainWindow = new MainWindow();
    m_mainWindow->showFullScreen();
    m_human = m_mainWindow->getHumanCharachter();
    m_human->setZValue(100);
    setupStateMachine();
    connect(m_mainWindow, &MainWindow::gameStarted, this, &GraphicalUI::gameStarted);
    setupShortcuts();
}

void GraphicalUI::createLevelUI(const std::vector<std::vector<Tile*>>& tiles) {
    m_mainWindow->getHumanCharachter()->setState(CharacterItem::State::Idle);
    for (const std::vector<Tile*>& row : tiles) {
        for (Tile* tile : row) {
            TileItem* tileUI = m_mainWindow->addTileToScene(tile->getRow(),
                                                            tile->getColumn(),
                                                            tile->getTexture());
            connect(tileUI, &TileItem::tilePressed, this, &GraphicalUI::moveCharacter);
            m_graphicalTiles.push_back(tileUI);

            if (tile->hasCharacter()) {
                if (tile->getCharacter()->isHuman()) {
                    m_human->setZValue(100);
                    CharacterTile_UI_PlacementMediator::PlaceCharacterOnTile(m_human, tileUI);
                    continue;
                }
                CharacterItem* charUI = new CharacterItem(CharacterItem::CharacterType::Goblin);
                charUI->setZValue(100);
                m_mainWindow->addCharacterToScene(charUI);
                CharacterTile_UI_PlacementMediator::PlaceCharacterOnTile(charUI, tileUI);
            }
        }
    }
}

void GraphicalUI::moveCharacter(TileItem* toWhichTile) {
    if (toWhichTile == m_human->getTile()) {
        return;
    }
    m_human->setState(CharacterItem::State::Walk, calculateMove(m_human->pos(), toWhichTile->pos()));
    m_human->setTile(toWhichTile);
}

void GraphicalUI::setVisualizationMode(VisualizationMode mode) { m_visualizationMode = mode; }

GraphicalUI::~GraphicalUI()
{
    delete startScreen;
    delete m_mainWindow;
}

void GraphicalUI::switchWindow()
{
    if (currentWindow == 0) {
        startScreen->hide();
        m_mainWindow->show();
        currentWindow = 1;
    } else {
        m_mainWindow->hide();
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
    QGameField* gameField = m_mainWindow->getGameField();
    m_overlayWidget  = new QOverlay(gameField, m_Qtiles);

    gameField->setChild(m_overlayWidget );
    m_overlayWidget ->setGeometry(gameField->rect());
    m_overlayWidget ->show();
    QGridLayout* gameBoard = m_mainWindow->getGameBoard();
    std::vector<std::vector<Tile*>> tiles = level->getTiles();
    for (int rowIterator = 0; rowIterator < level->getHeight(); rowIterator++) {
        for (int colIterator = 0; colIterator < level->getWidth(); colIterator++) {
            Tile* currentTile = (tiles)[rowIterator][colIterator];
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
                std::vector<std::pair<int,int>> path = {graph.generatePathFromPreviousRegister(previousRegister, loop.getExtractedTileCords(), LevelGraph::PathDirection::FromStartingToTarget, LevelGraph::PathCoordinateSystem::Absolute)};
                m_overlayWidget->addArrowPathBetweenMultipleTiles(path, true);
            }
        }
        return;
    }
    m_mainWindow->getArrowField()->setVisible(false);
    assert (m_graphMatrix!=nullptr && "Graph matrix is not available. ");
    int counter = 0;
    DjikstaInitialSetup(event);
    for (auto& loop : event->getLoops()){
        if (m_isVisualizeModeOn==false){break;}
        DjikstraVisualizeLoop(event, loop, counter++);
    }
    m_overlayWidget->highlightArrowPathAndRemoveOthers(event->getTargetTileCords(), event->getStartingTileCords());
    // m_overlayWidget->removeAllArrows();
    m_mainWindow->getArrowField()->setVisible(true);
    // setNoTextDjikstraMode(true);
    m_graphMatrix->resetMatrix();
    m_algorithmStepExplainerField->setText("");

}

void GraphicalUI::onCharacterHealthChange(CharacterHealthChangeEvent* event)
{
    // assert (event->getCharacter() == level->getPlayableCharacter() && "Wrong character");
    if (event->getCharacter()->isAlive()==false){
        QApplication::processEvents();
        playSound("qrc:/pics/textures/sounds/gameover.mp3", 1);

        m_mainWindow->gameOver();
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
    m_mainWindow->showTerminal();
    // JsonGenerator::saveGameState(dc->levels);
}

void GraphicalUI::setupStateMachine() {
    m_customizationAnimationLoop = new QSequentialAnimationGroup;

    m_stateMachine = new QStateMachine();

    QState* customizingState = new QState(m_stateMachine);
    QState* playingState = new QState(m_stateMachine);
    QFinalState* deadState = new QFinalState(m_stateMachine);

    connect(customizingState, &QState::entered, [this]() {
        m_mainWindow->getHumanCharachter()->setState(CharacterItem::State::Looping);
    });
    customizingState->addTransition(this, &GraphicalUI::gameStarted, playingState);

    m_stateMachine->setInitialState(customizingState);
    m_stateMachine->start();
}

void GraphicalUI::setupShortcuts() {
    QShortcut* shortcut = new QShortcut(QKeySequence(Qt::Key_Right), m_mainWindow);
}

QPointF GraphicalUI::calculateMove(QPointF from, QPointF to) {
    qreal x = to.x() - from.x();
    qreal y = to.y() - from.y();
    return QPointF{x, y};
}

std::pair<int, int> GraphicalUI::move()
{
    return lastMove;
}

void GraphicalUI::move(std::pair<int, int> xymove)
{
    m_mainWindow->findChild<QGraphMatrix*>("tileMatrixContainer")->resetMatrix();
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

void GraphicalUI::start() { m_mainWindow->show(); }

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

MainWindow* GraphicalUI::getMainWindow() { return m_mainWindow; }
