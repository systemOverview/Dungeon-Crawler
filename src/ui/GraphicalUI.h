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
    inline static Coordinates LAST_TILE_CLICKED_CORDS = {-1, -1};
    bool m_isVisualizeModeOn = true;
    MainWindow* m_mainWindow = nullptr;
    int currentWindow{0};
    Level* level;

    CharacterItem* m_human;

    std::pair<int, int> lastMove;
    std::vector<QWidget*> m_healthBars;

    QStateMachine* m_stateMachine = nullptr;
    QStateMachine* m_animationMachine = nullptr;

    QSequentialAnimationGroup* m_customizationAnimationLoop = nullptr;
    inline static std::map<Coordinates, TileItem*> GRAPHICAL_TILES = {};

    void startAnimationLoop();
    void setupStateMachine();
    void setupShortcuts();
    QPointF calculateMove(QPointF from, QPointF to);

public slots:
    void createLevelUI(const std::vector<std::vector<Tile*> >&);
    void moveCharacter(TileItem* toWhichTile);
    void tileClicked(TileItem* whichTile);
signals:
    void gameStarted();
    void humanHasInitiatedMove();

public:
    static Coordinates GetLastTileClickedCords();
    static TileItem* GetGraphicalTile(Coordinates tileCoordinates);
    GraphicalUI();
    MainWindow *getMainWindow();
    void quitVisualizationLoop();
    QWidget* generateHealthBar(int percentage, QWidget *parent);
    std::pair<int, int> move();
    void move(std::pair<int, int> xymove);
    void start();
    std::pair<int, int> getLastMove();
    void addHealthBar(QWidget *healthBar);
    void removeHealthBars();
    ~GraphicalUI();
    void switchWindow();
    void saveGame();
};

#endif // GRAPHICALUI_H
