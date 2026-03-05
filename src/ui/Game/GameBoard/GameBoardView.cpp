#include "GameBoardView.h"
#include <QtWidgets>
#include "CharacterItem.h"
#include "CharacterTile_UI_PlacementMediator.h"
#include "TileItem.h"
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
    connect(this, &GameBoardView::boardCellSizeChanged, tileView, &GameItem::setSideLength);
    return tileView;
}

const TileItem* GameBoardView::getTileViewByCoordinates(Coordinates coordinates) const {
    return *(m_tileViews.find(coordinates));
}

CharacterItem* GameBoardView::createCharacterView(Types::CharacterType characterType,
                                                  int characterId,
                                                  Coordinates initialTileCoordinates) {
    CharacterItem* characterView = new CharacterItem(characterType, characterId);
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
