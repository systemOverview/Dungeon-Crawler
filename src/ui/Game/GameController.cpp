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
#include "MainWindow.h"
#include "TileItem.h"
#include <MoveAnimation.h>

GameController::GameController() {
    m_gameEngine = new GameModelEngine();

    m_gameView = new GameBoardView();
    connect(m_gameView, &GameBoardView::tileClicked, this, &GameController::tileClicked);

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
        CharacterItem* characterView
            = m_gameView->createCharacterView(characterModel->getCharacterType(),
                                              characterModel->getCharacterID(),
                                              characterModel->getTile()->getCoordinates());
        connect(characterModel,
                &Character::characterMoved,
                m_gameView,
                &GameBoardView::moveCharacterView);
    }
}

void GameController::tileClicked(TileItem* whichTile) {
    LAST_TILE_CLICKED_CORDS = whichTile->getCoordinates();
    m_gameEngine->askCharactersToMove();
    emit humanHasInitiatedMove(); //connected to &DungeonCrawler::move;
}

Coordinates GameController::GetLastTileClickedCords() { return LAST_TILE_CLICKED_CORDS; }

TileItem* GameController::GetGraphicalTile(Coordinates tileCoordinates) {
    return GRAPHICAL_TILES.at(tileCoordinates);
}

CharacterItem* GameController::createCharacterUI(Types::CharacterType characterType,
                                                 Coordinates tileCoordinates,
                                                 int characterID) {
    CharacterItem* characterView = new CharacterItem(characterType, characterID);
    GRAPHICAL_CHARACTERS.insert({characterID, characterView});
    characterView->setZValue(100);
    return characterView;
}

GameController::~GameController() {}

void GameController::customEvent(QEvent* event) {
    if (event->type() == FightEvent::type()) {
        FightEvent* fightEvent = static_cast<FightEvent*>(event);
        animateFight(fightEvent);
    }
}

void GameController::animateFight(FightEvent* fightEvent) {
    for (FightRound fightRound : fightEvent->getFightRounds()) {
        animateFightRound(fightRound);
    }
}

void GameController::animateFightRound(FightRound fightRound) {
    CharacterItem* attackerView = GRAPHICAL_CHARACTERS.at(
        fightRound.getAttackingCharacterInfo().getCharacterID());
    FightAttackingAnimation* attackerAnimation = new FightAttackingAnimation(attackerView);

    CharacterItem* defenderView = GRAPHICAL_CHARACTERS.at(
        fightRound.getDefendingCharacterInfo().getCharacterID());
    FightDefendingAnimation* defenderAnimation
        = new FightDefendingAnimation(defenderView,
                                      fightRound.getDefendingCharacterInfo().getHealthPostRound());

    connect(attackerAnimation,
            &FightAttackingAnimation::iPunchedDefender,
            defenderAnimation,
            &FightDefendingAnimation::attackerPunchedMe);

    if (fightRound.getFightRoundOutcome() == FightRound::FightRoundOutcome::DefenderKilled) {
        connect(defenderAnimation, &FightDefendingAnimation::finished, [defenderView]() mutable {
            delete defenderView;
            defenderView = nullptr;
        });
    }
    defenderView->addAnimationToQueue(defenderAnimation);
    attackerView->addAnimationToQueue(attackerAnimation);
}

void GameController::start() {}

QGraphicsView* GameController::getGameBoardView() const { return m_gameView->getViewWidget(); }
