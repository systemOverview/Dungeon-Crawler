#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QEvent>
#include <QGridLayout>
#include <QMainWindow>
#include <QtWidgets/qgraphicsview.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qtoolbox.h>
#include "CharacterItem.h"
#include "EventBus.h"
#include "Level.h"
#include "QGameField.h"
#include "SpriteManager.h"
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
    enum MoveDirection {
        TopLeft,
        TopCenter,
        TopRight,
        CenterLeft,
        CenterCenter,
        CenterRight,
        BottomLeft,
        BottomCenter,
        BottomRight
    };

public slots:
    void arrowClicked(MainWindow::MoveDirection moveDirection);
    void armorButtonClicked(int armorID);
    void characterCustomizationClicked(SpriteManager::WhichSprite, int whichOption);
signals:
    void viewResized(QRect newViewRect);
    void characterMove(QPointF newPos);

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
    QGraphicsScene* m_gameScene = nullptr;
    QGraphicsView* m_gameView = nullptr;
    QGraphicsScene* m_startScene = nullptr;
    QGraphicsView* m_startView = nullptr;

    QWidget* m_sidebar = nullptr;
    QVBoxLayout* m_sidebarLayout = nullptr;
    QWidget* m_options = nullptr;
    QWidget* m_armorOptions = nullptr;

    QToolBox* m_sidebarToolBox = nullptr;

    CharacterItem* m_character;

    void setupShortcuts();
    void loadDefaultTiles();
    void makeStartScreen();
    void showCharOptions();
    void createArmorOptions();
    void createCharacterCustomizationOptions();

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
