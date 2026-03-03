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
class GraphicalUI : public QObject
{
    Q_OBJECT
public:

private:
    MainWindow* m_mainWindow = nullptr;

    inline static Coordinates LAST_TILE_CLICKED_CORDS = {-1, -1};

    QStateMachine* m_stateMachine = nullptr;
    QStateMachine* m_animationMachine = nullptr;

    inline static std::map<Coordinates, TileItem*> GRAPHICAL_TILES = {};
    inline static std::map<int, CharacterItem*> GRAPHICAL_CHARACTERS = {};

    void startAnimationLoop();
    void setupStateMachine();
    void setupShortcuts();

    void customEvent(QEvent* event) override;
    void animateFight(FightEvent* fightEvent);
    void animateFightRound(FightRound fightRound);

public slots:
    void createLevelView(const std::vector<std::vector<Tile*> >&);
    void tileClicked(TileItem* whichTile);
    void moveCharacterView(int characterID, Coordinates from, Coordinates to);
signals:
    void gameStarted();
    void humanHasInitiatedMove();

public:
    GraphicalUI();

    void start();

    static Coordinates GetLastTileClickedCords();
    static TileItem* GetGraphicalTile(Coordinates tileCoordinates);
    CharacterItem* createCharacterUI(Character::CharacterType characterType,
                                     Coordinates tileCoordinates,
                                     int characterID);
    ~GraphicalUI();
};
#endif // GRAPHICALUI_H
