#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QEvent>
#include <QGridLayout>
#include <QMainWindow>
#include <QtWidgets/qlineedit.h>
#include "Level.h"
#include "EventBus.h"
#include "QGameField.h"

class GraphicalUI;
class DungeonCrawler;
class Arrow
{
    std::pair<int, int> xymove;
    std::string path;

public:
    Arrow(std::pair<int, int> xy, std::string p)
    {
        xymove = xy;
        path = p;
    }

    std::pair<int, int> getMove() { return xymove; };
    std::string getPath() { return path; };
    DungeonCrawler *dc;
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public EventListener
{
    Q_OBJECT

public:
    explicit MainWindow(Level *lvl, GraphicalUI *g, QWidget *parent = nullptr);
    void generateArrowButtons(QGridLayout *arrowsField);

    ~MainWindow();
    void move(std::pair<int, int> move);

    QGridLayout *getGameBoard() const ;
    QGameField* getGameField() const;
    QWidget* getArrowField() const;
    void prepareRightSideForVisualization();
    void generateVisualizationWidgets(QWidget* containingWidget);
    void gameOver();
    void gameWon();
    void showTerminal();

private:
    Ui::MainWindow* ui;
    Level *level;
    QGridLayout* m_gameBoard;
    QGameField* m_gameField;
    QWidget* m_arrowField;
    GraphicalUI *GUI;
    std::array<Arrow *, 9> arrows = {
        new Arrow({-1, -1}, ":/pics/textures/arrows/arrow_up_left.png"),
        new Arrow({-1, 0}, ":/pics/textures/arrows/arrow_up.png"),
        new Arrow({-1, 1}, ":/pics/textures/arrows/arrow_up_right.png"),
        new Arrow({0, -1}, ":/pics/textures/arrows/arrow_left.png"),
        new Arrow({0, 0}, ":/pics/textures/arrows/arrow_skip.png"),
        new Arrow({0, 1}, ":/pics/textures/arrows/arrow_right.png"),
        new Arrow({1, -1}, ":/pics/textures/arrows/arrow_down_left.png"),
        new Arrow({1, 0}, ":/pics/textures/arrows/arrow_down.png"),
        new Arrow({1, 1}, ":/pics/textures/arrows/arrow_down_right.png"),

    };
};

#endif // MAINWINDOW_H
