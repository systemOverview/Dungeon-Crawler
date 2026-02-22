#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QEvent>
#include <QGridLayout>
#include <QMainWindow>
#include <QtWidgets/qgraphicsview.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qtoolbox.h>
#include "ButtonItem.h"
#include "CharacterItem.h"
#include "EventBus.h"
#include "Level.h"
#include "QGameField.h"
#include "SpriteManager.h"
class GraphicalUI;
class DungeonCrawler;
class TileItem;
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

private:
    QGraphicsScene* m_scene = nullptr;
    QGraphicsView* m_view = nullptr;

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
    void fillGameBoard();

    qreal calculateTextureDimension();
    Ui::MainWindow* ui;
    Level* level;
    QGridLayout* m_gameBoard;
    QGameField* m_gameField;
    QWidget* m_arrowField;
    GraphicalUI* GUI;
    std::array<Arrow*, 9> arrows = {
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

public slots:
    void characterCustomizationClicked(CharacterItem::CharacterPart characterPart, int whichOption);
    void startGame();
signals:
    void dimensionsChanged();
    void characterMove(QPointF newPos);
    void gameStarted();

public:
    explicit MainWindow(QWidget* parent = nullptr);
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
    TileItem* addTileToScene(int row, int col, char textureID);
    void addCharacterToScene(CharacterItem* character) const;
    CharacterItem* getHumanCharachter() const;
    bool eventFilter(QObject* obj, QEvent* event) override;
};

#endif // MAINWINDOW_H
