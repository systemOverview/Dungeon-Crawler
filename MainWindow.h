#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include "Level.h"
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

    //     case 1:
    //         xymove ={1,-1};
    //         return xymove;
    //     case 2:
    //         xymove = {1,0};
    //         return xymove;
    //     case 3:
    //         xymove = {1,1}; /*1-3, 2,2*/
    //         return xymove;
};






// std::pair<int, int> TerminalUI::translateMove(int step) {
//     std::pair<int,int> xymove;
//     switch (step) {
//         /* to go down, you add +1 to the row, to go up -1 the row, right +1 the column, left -1 the column*/
//     case 1:
//         xymove ={1,-1};
//         return xymove;
//     case 2:
//         xymove = {1,0};
//         return xymove;
//     case 3:
//         xymove = {1,1}; /*1-3, 2,2*/
//         return xymove;
//     case 4:
//         xymove = {0,-1};
//         return xymove;
//     case 5:
//         xymove = {0,0};
//         return xymove;
//     case 6:
//         xymove = {0,1};
//         return xymove;
//     case 7:
//         xymove = {-1,-1};
//         return xymove;
//     case 8:
//         xymove = {-1,0};
//         return xymove;
//     case 9:
//         xymove = {-1,1};
//         return xymove;

//     }
// }

#endif // MAINWINDOW_H
