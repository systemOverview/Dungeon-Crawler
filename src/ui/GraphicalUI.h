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
    std::vector<CharacterItem*> graphicalCharacters = {};

    void startAnimationLoop();
    void setupStateMachine();
    void setupShortcuts();

public slots:
    void createLevelUI(const std::vector<std::vector<Tile*> >&);
    void tileClicked(TileItem* whichTile);
signals:
    void gameStarted();
    void humanHasInitiatedMove();

public:
    GraphicalUI();

    void start();

    static Coordinates GetLastTileClickedCords();
    static TileItem* GetGraphicalTile(Coordinates tileCoordinates);
    CharacterItem* createCharacterUI(Character::CharacterType characterType,
                                     Coordinates tileCoordinates);
    ~GraphicalUI();
};

#endif // GRAPHICALUI_H
