#ifndef GRAPHICALUI_H
#define GRAPHICALUI_H
#include <QObject>
#include "Constants.h"
#include <AppPageController.h>
class GameBoardView;
class GameModelEngine;
class TileItem;
class Level;
class GameController : public QObject, public AppPageController
{
    Q_OBJECT

private:
    GameModelEngine* m_gameEngine = nullptr;
    GameBoardView* m_gameView = nullptr;

    inline static Coordinates LAST_TILE_CLICKED_CORDS = {-1, -1};

public slots:
    void tileClicked(TileItem* whichTile);
signals:
    void gameStarted();
    void tileModelReplaced(
        Coordinates replacedTileCoordinates,
        Types::TileType newTileType); // connects Level::tileReplaced to GameBoard::replaceTileView

public:
    enum class GameSource { DefaultLevels, CustomLevels };

    GameController();

    void startNewGame(GameSource gameSource);

    void createLevelView(Level* level);

    static Coordinates GetLastTileClickedCords();

    ~GameController();

    // AppPageController interface
public:
    QWidget* getWidget() override;

protected:
    void createActions() override;
};
#endif // GRAPHICALUI_H
