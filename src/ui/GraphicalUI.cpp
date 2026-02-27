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
#include "MainWindow.h"
#include "TileItem.h"

GraphicalUI::GraphicalUI() {
    m_mainWindow = new MainWindow();
    m_mainWindow->showFullScreen();

    setupStateMachine();

    connect(m_mainWindow,
            &MainWindow::gameStarted,
            this,
            &GraphicalUI::gameStarted); // both are signals, routes game started from Main Window to Dungeon Crawler

    setupShortcuts();
}

void GraphicalUI::createLevelUI(const std::vector<std::vector<Tile*>>& tiles) {
    for (const std::vector<Tile*>& row : tiles) {
        for (Tile* tile : row) {
            TileItem* tileView = new TileItem(tile->getRow(), tile->getColumn(), tile->getTexture());
            m_mainWindow->addGameItemToScene(tileView);

            GRAPHICAL_TILES.insert({tileView->getCoordinates(), tileView});

            connect(tileView, &TileItem::tilePressed, this, &GraphicalUI::tileClicked);
        }
    }
    for (CharacterItem* characterView : graphicalCharacters) {
        m_mainWindow->addGameItemToScene(characterView);
        characterView->setZValue(100);
        characterView->fixMyPosition();
    }
}

void GraphicalUI::tileClicked(TileItem* whichTile) {
    LAST_TILE_CLICKED_CORDS = whichTile->getCoordinates();
    emit humanHasInitiatedMove(); //connected to &DungeonCrawler::move;
}

Coordinates GraphicalUI::GetLastTileClickedCords() { return LAST_TILE_CLICKED_CORDS; }

TileItem* GraphicalUI::GetGraphicalTile(Coordinates tileCoordinates) {
    return GRAPHICAL_TILES.at(tileCoordinates);
}

CharacterItem* GraphicalUI::createCharacterUI(Character::CharacterType characterType,
                                              Coordinates tileCoordinates) {
    CharacterItem* character = new CharacterItem(characterType);

    if (characterType == Character::CharacterType::Human) {
        character->setPartsGraphics(m_mainWindow->getHumanPartsGraphics());
    }

    CharacterTile_UI_PlacementMediator::PlaceCharacterOnTile(character, tileCoordinates);
    graphicalCharacters.push_back(character);
    return character;
}

GraphicalUI::~GraphicalUI()
{
    delete m_mainWindow;
}


void GraphicalUI::setupStateMachine() {

    m_stateMachine = new QStateMachine();

    QState* customizingState = new QState(m_stateMachine);
    QState* playingState = new QState(m_stateMachine);
    QFinalState* deadState = new QFinalState(m_stateMachine);

    // connect(customizingState, &QState::entered, [this]() {
    //     m_mainWindow->getHumanCharachter()->setState(CharacterItem::State::Looping);
    // });
    customizingState->addTransition(this, &GraphicalUI::gameStarted, playingState);

    m_stateMachine->setInitialState(customizingState);
    m_stateMachine->start();
}

void GraphicalUI::setupShortcuts() {
    QShortcut* shortcut = new QShortcut(QKeySequence(Qt::Key_Right), m_mainWindow);
}

void GraphicalUI::start() { m_mainWindow->show(); }
