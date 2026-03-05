#ifndef GAMEBOARDVIEW_H
#define GAMEBOARDVIEW_H

#include <QObject>
#include "Constants.h"
#include "TileItem.h"

class QGraphicsScene;
class QGraphicsView;
class CharacterItem;
class CharacterTile_UI_PlacementMediator;
class GameBoardView : public QObject
{
    Q_OBJECT
    QGraphicsScene* m_scene = nullptr;
    QGraphicsView* m_view = nullptr;
    CharacterTile_UI_PlacementMediator* m_placementMediator = nullptr;

    QMap<Coordinates, TileItem*> m_tileViews{};
    QMap<int, CharacterItem*> m_characterViews{};

    qreal calculateBoardCellSideSize();
public slots:
    void replaceTileView(Coordinates replacedTileCoordinates, Types::TileType newTyleType);
    void moveCharacterView(int characterID, Coordinates from, Coordinates to);
signals:
    void boardCellSizeChanged(qreal newSideLength);
    void tileClicked(TileItem* whichTile);

public:
    GameBoardView();

    QGraphicsView* getViewWidget() const;
    CharacterTile_UI_PlacementMediator* getPlacementMediator() const;

    //Tile insertion/retrieval :
    TileItem* createTileView(Types::TileType tileType,
                             Coordinates tileCoordinates,
                             TileItem::Mode mode = TileItem::Mode::Clickable);
    const TileItem* getTileViewByCoordinates(Coordinates coordinates) const;

    CharacterItem* createCharacterView(Types::CharacterType characterType,
                                       int characterId,
                                       Coordinates initialTileCoordinates);
    void changeCharacterCoordinates(int characterID, Coordinates newCoordinates);

    bool eventFilter(QObject* obj, QEvent* event);
};

#endif // GAMEBOARDVIEW_H
