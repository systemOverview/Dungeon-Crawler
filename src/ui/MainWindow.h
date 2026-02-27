#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QEvent>
#include <QGridLayout>
#include <QMainWindow>
#include <QtWidgets/qgraphicsview.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qtoolbox.h>
#include "CharacterItem.h"
#include "Level.h"
class GraphicalUI;
class DungeonCrawler;
class TileItem;

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

    QWidget* m_arrowField;
    GraphicalUI* GUI;

    std::map<CharacterItem::CharacterPart, int> m_humanPartsGraphics;

public slots:
    void characterCustomizationClicked(CharacterItem::CharacterPart characterPart, int whichOption);
    void startGame();
signals:
    void dimensionsChanged();
    void gameStarted();

public:
    explicit MainWindow(QWidget* parent = nullptr);

    ~MainWindow();

    void addGameItemToScene(GameItem* gameItem) const;

    bool eventFilter(QObject* obj, QEvent* event) override;
    std::map<CharacterItem::CharacterPart, int> getHumanPartsGraphics() const;
};

#endif // MAINWINDOW_H
