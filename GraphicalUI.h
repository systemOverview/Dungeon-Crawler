#ifndef GRAPHICALUI_H
#define GRAPHICALUI_H
#include <QDialog>
#include <QMainWindow>
#include <QPainter>
#include "AbstractUI.h"
#include "Level.h"
#include "MainWindow.h"
#include "QTile.h"
#include "EventBus.h"

class DungeonCrawler;
class GraphicalUI : public AbstractUI, public AbstractController, public EventListener
{
private:
    bool m_isVisualizeModeOn=true;
    QDialog* startScreen;
    MainWindow *mainWindow;
    int currentWindow{0};
    QEventLoop* m_visualizationLoop;
    int m_loopDuration = 100;
    std::queue<AnimateTileEvent*> m_animationsQueue;
    Level* level;
    DungeonCrawler* dc;
    std::pair<int, int> lastMove;
    std::vector<QWidget *> m_healthBars;
    std::map<std::pair<int,int>, QTile*> m_Qtiles;
    std::vector<QTile*> m_temporarelyAlteredTiles;

public:
    GraphicalUI(Level *lvl, DungeonCrawler *d);
    QDialog *getStartScreen();
    QMainWindow *getMainWindow();
    void draw(Level *) override;
    void quitVisualizationLoop();
    QWidget* generateHealthBar(int percentage, QWidget *parent);
    std::pair<int, int> move() override;
    void move(std::pair<int, int> xymove);
    std::pair<int, int> translateMove(int step) override;
    void start();
    std::pair<int, int> getLastMove();
    void addHealthBar(QWidget *healthBar);
    void removeHealthBars();
    ~GraphicalUI();
    void playSound(QString soundLink, float volume);
    void switchWindow();
    void deleteAllTiles();
    void onAnimateTile(AnimateTileEvent* event) override;
    void onVisualizationChange(VisualizationStatusEvent* event) override;
};

#endif // GRAPHICALUI_H
