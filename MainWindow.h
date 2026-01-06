#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include "Level.h"
#include <QEvent>



class GraphicalUI;
class DungeonCrawler;
class Arrow{
    std::pair <int,int> xymove;
    std::string path;
public:
    Arrow(std::pair<int,int> xy, std::string p) {
        xymove = xy;
        path = p;
    }

    std::pair <int, int> getMove(){return xymove;};
    std::string getPath(){return path;};
    DungeonCrawler* dc;

};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Level* lvl,GraphicalUI* g
,  QWidget *parent = nullptr);
    void fill(QGridLayout* gameField);
    void generateArrowButtons(QGridLayout* arrowsField);

    ~MainWindow();
    void move(std::pair <int, int> move);

    QGridLayout* getGameBoard();
private:
    Ui::MainWindow *ui;
    Level* level;
    QGridLayout* gameBoard;
    GraphicalUI* GUI;

    std::array<Arrow*, 9> arrows = {
        new Arrow({-1,-1},":/pics/textures/arrows/arrow_up_left.png"),
        new Arrow({-1,0},":/pics/textures/arrows/arrow_up.png"),
        new Arrow({-1,1},":/pics/textures/arrows/arrow_up_right.png"),
        new Arrow({0,-1},":/pics/textures/arrows/arrow_left.png"),
        new Arrow({0,0},":/pics/textures/arrows/arrow_skip.png"),
        new Arrow({0,1},":/pics/textures/arrows/arrow_right.png"),
        new Arrow({1,-1},":/pics/textures/arrows/arrow_down_left.png"),
        new Arrow({1,0},":/pics/textures/arrows/arrow_down.png"),
        new Arrow({1,1},":/pics/textures/arrows/arrow_down_right.png"),









};

};







#endif // MAINWINDOW_H
