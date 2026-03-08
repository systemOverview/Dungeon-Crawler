#ifndef GRAPHICALUI_H
#define GRAPHICALUI_H
#include <QDialog>
#include <QMainWindow>
#include <QObject>
#include <QPainter>
#include <QTextEdit>
#include "Level.h"
#include "MainWindow.h"

class DungeonCrawler;
class FightEvent;
class FightRound;
class GameBoardView;
class CharacterTile_UI_PlacementMediator;
class GameModelEngine;
class GameController : public QObject
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
    QGraphicsView* getGameBoardView() const;

    static Coordinates GetLastTileClickedCords();

    ~GameController();
};
#endif // GRAPHICALUI_H
