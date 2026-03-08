#ifndef GAMEBOARDVIEW_H
#define GAMEBOARDVIEW_H

#include <QObject>
#include "Constants.h"
#include "TileItem.h"

class QGraphicsScene;
class QGraphicsView;
class CharacterItem;
class CharacterTile_UI_PlacementMediator;
class FightEvent;
class FightRound;
class GameBoardView : public QObject
{
private:
    Q_OBJECT
    QGraphicsScene* m_scene = nullptr;
    QGraphicsView* m_view = nullptr;
    CharacterTile_UI_PlacementMediator* m_placementMediator = nullptr;

    QMap<Coordinates, TileItem*> m_tileViews{};
    QMap<int, CharacterItem*> m_characterViews{};

    qreal calculateBoardCellSideSize();
    void animateFightRound(FightRound fightRound);

public slots:
    void replaceTileView(Coordinates replacedTileCoordinates, Types::TileType newTyleType);
    void moveCharacterView(int characterID, Coordinates from, Coordinates to);
    void animateFight(FightEvent* fightEvent);

signals:
    void boardCellSizeChanged(qreal newSideLength);
    void tileClicked(TileItem* whichTile);

public:
    GameBoardView();

    QGraphicsView* getViewWidget() const;
    CharacterTile_UI_PlacementMediator* getPlacementMediator() const;

    TileItem* createTileView(Types::TileType tileType,
                             Coordinates tileCoordinates,
                             TileItem::Mode mode = TileItem::Mode::Clickable);
    const TileItem* getTileViewByCoordinates(Coordinates coordinates) const;

    CharacterItem* createCharacterView(Types::CharacterType characterType,
                                       int characterId,
                                       Coordinates initialTileCoordinates,
                                       bool isCharacterHealthBarShown = false);
    void changeCharacterCoordinates(int characterID, Coordinates newCoordinates);

    bool eventFilter(QObject* obj, QEvent* event);
};

#endif // GAMEBOARDVIEW_H
