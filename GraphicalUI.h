#ifndef GRAPHICALUI_H
#define GRAPHICALUI_H
#include "AbstractUI.h"
#include <QDialog>
#include <QMainWindow>
// #include "startscreen.h"
// #include "DungeonCrawler.h"
#include "MainWindow.h"
#include "Level.h"
class DungeonCrawler;
class GraphicalUI : public AbstractUI, public AbstractController
{
private:
    QDialog* startScreen;
    MainWindow* mainWindow;
    int currentWindow {0};
    Level* level;
    DungeonCrawler* dc;
    std::pair <int, int> lastMove;

public:
    GraphicalUI(Level* lvl, DungeonCrawler* d);
    QDialog* getStartScreen();
    QMainWindow* getMainWindow();
    void draw (Level*) override;
    int move(Level*) override;
    int move(std::pair<int,int> xymove);
    std::pair<int,int> translateMove(int step) override;
    void start();
    std::pair <int, int> getLastMove();
    ~GraphicalUI();
public slots:
    void switchWindow();

};

#endif // GRAPHICALUI_H
