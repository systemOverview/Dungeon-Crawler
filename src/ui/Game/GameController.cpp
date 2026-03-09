#include "GameController.h"
#include <QtWidgets/qapplication.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qmessagebox.h>
#include <QtWidgets/qpushbutton.h>
#include "Character.h"
#include "GameBoardView.h"
#include "GameModelEngine.h"
#include "Level.h"
#include "QGraphicsView"
#include "TileItem.h"

GameController::GameController() { m_gameView = new GameBoardView(); }

void GameController::startNewGame(GameSource gameSource) {
    m_gameEngine = new GameModelEngine();

    if (gameSource == GameSource::CustomLevels) {
        m_gameEngine->createGameFromJson(DataPaths::GetCustomLevelPath());
    }

    else {
        m_gameEngine->createGameFromJson(DataPaths::DefaultLevelPath);
    }

    connect(m_gameView, &GameBoardView::tileClicked, this, &GameController::tileClicked);

    connect(m_gameEngine, &GameModelEngine::fightErupted, m_gameView, &GameBoardView::animateFight);

    createLevelView(m_gameEngine->getCurrentLevel());
}
void GameController::createActions() {
    GameAction* startDefaultGame = new GameAction(GameAction::ActionID::LoadDefaultGame);

    connect(startDefaultGame, &QAction::triggered, [this]() {
        startNewGame(GameSource::DefaultLevels);
    });

    startDefaultGame->setText("Start a new game.");
    m_actions.push_back(startDefaultGame);

    GameAction* resumeSavedGame = new GameAction(GameAction::ActionID::LoadSavedGame);

    connect(resumeSavedGame, &QAction::triggered, [this]() {
        startNewGame(GameSource::CustomLevels);
    });

    resumeSavedGame->setText("Resume saved game.");
    m_actions.push_back(resumeSavedGame);
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


GameController::~GameController() {}

QWidget* GameController::getWidget() { return m_gameView->getViewWidget(); }
