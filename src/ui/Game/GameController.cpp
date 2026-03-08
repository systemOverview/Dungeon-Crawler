#include "GameController.h"
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
#include "FightAnimation.h"
#include "FightEvent.h"
#include "GameBoardView.h"
#include "GameModelEngine.h"
#include "JsonObjects.h"
#include "MainWindow.h"
#include "TileItem.h"
#include <MoveAnimation.h>

GameController::GameController() { m_gameView = new GameBoardView(); }

void GameController::startNewGame(GameSource gameSource) {
    m_gameEngine = new GameModelEngine();

    if (gameSource == GameSource::CustomLevels) {
        json jsonInfo = JsonGenerator::parseFileJson(DataPaths::GetCustomLevelPath());
        m_gameEngine->createGameFromJson(jsonInfo);
    }
    else {
        m_gameEngine->createGameFromString(GameData::GameStrings[0]);
    }

    connect(m_gameView, &GameBoardView::tileClicked, this, &GameController::tileClicked);

    connect(m_gameEngine, &GameModelEngine::fightErupted, m_gameView, &GameBoardView::animateFight);

    createLevelView(m_gameEngine->getCurrentLevel());
}

void GameController::createLevelView(Level* level) {
    connect(level, &Level::tileReplaced, [this](Coordinates replacedTileCoordinates, Tile* newTile) {
        m_gameView->replaceTileView(replacedTileCoordinates, newTile->getTileType());
    });

    for (const std::vector<Tile*>& row : level->getTiles()) {
        for (Tile* tileModel : row) {
            TileItem* tileView = m_gameView->createTileView(tileModel->getTileType(),
                                                            tileModel->getCoordinates());

            connect(tileModel, &Tile::tileTypeChanged, tileView, &TileItem::changeTileType);
        }
    }
    for (Character* characterModel : level->getCharacters()) {
        m_gameView->createCharacterView(characterModel->getCharacterType(),
                                        characterModel->getCharacterID(),
                                        characterModel->getTile()->getCoordinates(),
                                        true);
        connect(characterModel,
                &Character::characterMoved,
                m_gameView,
                &GameBoardView::moveCharacterView);
    }
}

void GameController::tileClicked(TileItem* whichTile) {
    LAST_TILE_CLICKED_CORDS = whichTile->getCoordinates();
    m_gameEngine->askCharactersToMove();
}
Coordinates GameController::GetLastTileClickedCords() { return LAST_TILE_CLICKED_CORDS; }

QGraphicsView* GameController::getGameBoardView() const { return m_gameView->getViewWidget(); }

GameController::~GameController() {}
