#include "GameBoardView.h"
#include <QtWidgets>
#include "CharacterItem.h"
#include "CharacterTile_UI_PlacementMediator.h"
#include "FightEvent.h"
#include "TileItem.h"
#include <FightAnimation.h>
#include <FightAnimationMediator.h>
#include <MoveAnimation.h>

GameBoardView::GameBoardView() {
    m_scene = new QGraphicsScene(this);

    m_view = new QGraphicsView(m_scene);
    m_view->installEventFilter(this);
    m_view->setFrameStyle(QFrame::NoFrame);

    m_placementMediator = new CharacterTile_UI_PlacementMediator(this);
}

QGraphicsView* GameBoardView::getViewWidget() const { return m_view; }

CharacterTile_UI_PlacementMediator* GameBoardView::getPlacementMediator() const {
    return m_placementMediator;
}

TileItem* GameBoardView::createTileView(Types::TileType tileType,
                                        Coordinates tileCoordinates,
                                        TileItem::Mode mode) {
    TileItem* tileView = new TileItem(tileType, tileCoordinates, mode);
    m_scene->addItem(tileView);

    m_tileViews.insert(tileCoordinates, tileView);

    connect(tileView, &TileItem::tileClicked, this, &GameBoardView::tileClicked);
    connect(this, &GameBoardView::boardCellSizeChanged, tileView, &GameItem::setSideLength);

    return tileView;
}

const TileItem* GameBoardView::getTileViewByCoordinates(Coordinates coordinates) const {
    return *(m_tileViews.find(coordinates));
}

CharacterItem* GameBoardView::createCharacterView(Types::CharacterType characterType,
                                                  int characterId,
                                                  Coordinates initialTileCoordinates,
                                                  bool isCharacterHealthBarShown) {
    CharacterItem* characterView = new CharacterItem(characterType,
                                                     characterId,
                                                     isCharacterHealthBarShown);
    connect(this, &GameBoardView::boardCellSizeChanged, characterView, &GameItem::setSideLength);

    characterView->setSideLength(
        m_tileViews.cbegin()
            .value()
            ->boundingRect()
            .width());// incase no resize happens when character is inserted.

    m_characterViews.insert(characterId, characterView);
    m_scene->addItem(characterView);
    m_placementMediator->placeCharacterOnTile(characterView, initialTileCoordinates);

    return characterView;
}

void GameBoardView::changeCharacterCoordinates(int characterID, Coordinates newCoordinates) {
    CharacterItem* characterView = *(m_characterViews.find(characterID));
    m_placementMediator->placeCharacterOnTile(characterView, newCoordinates);
}

void GameBoardView::moveCharacterView(int characterID, Coordinates from, Coordinates to) {
    CharacterItem* characterView = *(m_characterViews.find(characterID));

    characterView->addAnimationToQueue(
        new MoveAnimation(characterView, from, to, m_placementMediator));
}

//Resizing functions :

bool GameBoardView::eventFilter(QObject* obj, QEvent* event) {
    if (event->type() == QEvent::Resize) {
        QGraphicsView* view = qobject_cast<QGraphicsView*>(obj);
        if (view) {
            emit boardCellSizeChanged(calculateBoardCellSideSize());
        }
        m_placementMediator->recalculateCharactersScenePositions();
        m_scene->setSceneRect(m_scene->itemsBoundingRect());
    }
    return false;
}

qreal GameBoardView::calculateBoardCellSideSize() {
    return std::min(m_view->rect().width() / GameSettings::TILES_PER_SIDE,
                    m_view->rect().height() / GameSettings::TILES_PER_SIDE);
}

void GameBoardView::replaceTileView(Coordinates replacedTileCoordinates, Types::TileType newTyleType) {
    assert(m_tileViews.count(replacedTileCoordinates) > 0 && "Level view and model mismatch");
    (m_tileViews.find(replacedTileCoordinates)).value()->changeTileType(newTyleType);
    qDebug() << m_scene->items().count();
    // TileItem*
}

void GameBoardView::animateFight(FightEvent* fightEvent) {
    if (fightEvent->getFightRounds().size() == 0) {
        return;
    }
    CharacterItem* firstCharacter = *(m_characterViews.find(
        fightEvent->getFightRounds().at(0).getAttackingCharacterInfo().getCharacterID()));

    CharacterItem* secondCharacter = *(m_characterViews.find(
        fightEvent->getFightRounds().at(0).getDefendingCharacterInfo().getCharacterID()));

    QMap<int, CharacterItem*> fightingCharacters
        = {{firstCharacter->getCharacterID(), firstCharacter},
           {secondCharacter->getCharacterID(),
            secondCharacter}}; // only pass the characters fight animation mediator needs.

    FightAnimationMediator* firstRoundAnimation = new FightAnimationMediator(fightEvent,
                                                                             fightingCharacters);

    firstRoundAnimation->start();
    // for (FightRound fightRound : fightEvent->getFightRounds()) {
    //     animateFightRound(fightRound);
    // }
}

void GameBoardView::animateFightRound(FightRound fightRound) {
    CharacterItem* attackerView = *(
        m_characterViews.find(fightRound.getAttackingCharacterInfo().getCharacterID()));

    FightAttackingAnimation* attackerAnimation = new FightAttackingAnimation(attackerView);

    CharacterItem* defenderView = *(
        m_characterViews.find(fightRound.getDefendingCharacterInfo().getCharacterID()));
    FightDefendingAnimation* defenderAnimation
        = new FightDefendingAnimation(defenderView,
                                      fightRound.getDefendingCharacterInfo().getHealthPostRound());

    defenderView->lookTowardsPosition(attackerView->pos());

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

    return;

    // Utilities::QtSleepMilliSeconds(7000);
}
