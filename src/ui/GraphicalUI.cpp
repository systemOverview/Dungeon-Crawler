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

GraphicalUI::GraphicalUI() {
    m_mainWindow = new MainWindow();
    m_mainWindow->showFullScreen();

    m_human = m_mainWindow->getHumanCharachter();
    m_human->setZValue(100);

    setupStateMachine();

    connect(m_mainWindow,
            &MainWindow::gameStarted,
            this,
            &GraphicalUI::gameStarted); // both are signals, routes game started from Main Window to Dungeon Crawler

    setupShortcuts();
}

void GraphicalUI::createLevelUI(const std::vector<std::vector<Tile*>>& tiles) {
    m_mainWindow->getHumanCharachter()->setState(CharacterItem::State::Idle);
    for (const std::vector<Tile*>& row : tiles) {
        for (Tile* tile : row) {
            TileItem* graphicalTile = m_mainWindow->addTileToScene(tile->getRow(),
                                                                   tile->getColumn(),
                                                                   tile->getTexture());

            GRAPHICAL_TILES.insert({graphicalTile->getCoordinates(), graphicalTile});
            connect(graphicalTile, &TileItem::tilePressed, this, &GraphicalUI::tileClicked);

            if (tile->getCharacter()) {
                if (tile->getCharacter()->isHuman()) {
                    m_human->setZValue(100);
                    CharacterTile_UI_PlacementMediator::PlaceCharacterOnTile(m_human, graphicalTile);
                    DungeonCrawler::ConnectGeneratedCharacter(tile->getCharacter(), m_human);
                    continue;
                }
                CharacterItem* charUI = new CharacterItem(CharacterItem::CharacterType::Goblin);
                charUI->setZValue(100);
                m_mainWindow->addCharacterToScene(charUI);
                CharacterTile_UI_PlacementMediator::PlaceCharacterOnTile(charUI, graphicalTile);
                DungeonCrawler::ConnectGeneratedCharacter(tile->getCharacter(), charUI);
            }
        }
    }
}

void GraphicalUI::moveCharacter(TileItem* toWhichTile) {
}

void GraphicalUI::tileClicked(TileItem* whichTile) {
    // This function does an equality check on the requested tile and prepares for emitting humanHasInitiatedMove,
    // it saves the last tile the human clicked that would be requestes by its controller

    if (whichTile == m_human->getTile()) {
        return;
    }
    LAST_TILE_CLICKED_CORDS = whichTile->getCoordinates();
    emit humanHasInitiatedMove(); //connected to &DungeonCrawler::move;
}

Coordinates GraphicalUI::GetLastTileClickedCords() { return LAST_TILE_CLICKED_CORDS; }

TileItem* GraphicalUI::GetGraphicalTile(Coordinates tileCoordinates) {
    return GRAPHICAL_TILES.at(tileCoordinates);
}


GraphicalUI::~GraphicalUI()
{
    delete m_mainWindow;
}

void GraphicalUI::saveGame()
{
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


MainWindow* GraphicalUI::getMainWindow() { return m_mainWindow; }
