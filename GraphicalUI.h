#ifndef GRAPHICALUI_H
#define GRAPHICALUI_H
#include <QDialog>
#include <QMainWindow>
#include <QPainter>
#include "AbstractUI.h"
#include "Level.h"
#include "MainWindow.h"

class DungeonCrawler;
class GraphicalUI : public AbstractUI, public AbstractController
{
private:
    QDialog *startScreen;
    MainWindow *mainWindow;
    int currentWindow{0};
    Level *level;
    DungeonCrawler *dc;
    std::pair<int, int> lastMove;
    std::vector<QWidget *> m_healthBars;

public:
    GraphicalUI(Level *lvl, DungeonCrawler *d);
    QDialog *getStartScreen();
    QMainWindow *getMainWindow();
    void draw(Level *) override;
    QWidget *generateHealthBar(int percentage, QWidget *parent);
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
};

#endif // GRAPHICALUI_H
