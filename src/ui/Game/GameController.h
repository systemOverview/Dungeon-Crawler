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

    inline static std::map<Coordinates, TileItem*> GRAPHICAL_TILES = {};
    inline static std::map<int, CharacterItem*> GRAPHICAL_CHARACTERS = {};

    void startAnimationLoop();

    void customEvent(QEvent* event) override;
    void animateFight(FightEvent* fightEvent);
    void animateFightRound(FightRound fightRound);
    CharacterItem* createCharacterUI(Types::CharacterType characterType,
                                     Coordinates tileCoordinates,
                                     int characterID);

public slots:
    void tileClicked(TileItem* whichTile);
signals:
    void gameStarted();
    void tileModelReplaced(
        Coordinates replacedTileCoordinates,
        Types::TileType newTileType); // connects Level::tileReplaced to GameBoard::replaceTileView
    void humanHasInitiatedMove();

public:
    GameController();
    void createLevelView(Level* level);
    void start();
    QGraphicsView* getGameBoardView() const;

    static Coordinates GetLastTileClickedCords();
    static TileItem* GetGraphicalTile(Coordinates tileCoordinates);
    ~GameController();
};
#endif // GRAPHICALUI_H
