#ifndef GRAPHICALUI_H
#define GRAPHICALUI_H
#include <QDialog>
#include <QMainWindow>
#include <QObject>
#include <QPainter>
#include <QTextEdit>
#include "AbstractUI.h"
#include "EventBus.h"
#include "Level.h"
#include "List.tpp"
#include "MainWindow.h"
#include "QGraphMatrix.h"
#include "QTile.h"
#include "QTypeWriter.h"
#include "Utilities.h"
class DungeonCrawler;
class GraphicalUI : public QObject,
                    public AbstractUI,
                    public AbstractController,
                    public EventListener
{
    Q_OBJECT
public:
    enum VisualizationMode { FullVisualization, FullVisualizationWithoutText, OnlyFinalPath, None };

private:
    bool m_isVisualizeModeOn=true;
    QDialog* startScreen;
    MainWindow* m_mainWindow = nullptr;
    int currentWindow{0};
    Level* level;

    CharacterItem* m_human;

    DungeonCrawler* dc;
    std::pair<int, int> lastMove;
    std::vector<QWidget *> m_healthBars;
    std::map<std::pair<int,int>, QTile*> m_Qtiles;
    QOverlay* m_overlayWidget = nullptr;
    QGraphMatrix* m_graphMatrix = nullptr;
    QTypeWriter* m_algorithmStepExplainerField = nullptr;
    VisualizationMode m_visualizationMode = FullVisualization;

    QStateMachine* m_stateMachine = nullptr;
    QStateMachine* m_animationMachine = nullptr;

    QSequentialAnimationGroup* m_customizationAnimationLoop = nullptr;
    List<TileItem*> m_graphicalTiles;

    void startAnimationLoop();
    void setupStateMachine();
    void setupShortcuts();
    QPointF calculateMove(QPointF from, QPointF to);

public slots:
    void createLevelUI(const std::vector<std::vector<Tile*> >&);
    void moveCharacter(TileItem* toWhichTile);
signals:
    void gameStarted();

public:
    void setVisualizationMode(VisualizationMode mode);
    GraphicalUI();
    QDialog *getStartScreen();
    MainWindow *getMainWindow();
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
    // Event Functions
    void onDjikstraSearch(DjikstraSearchEvent* event) override;
    void onCharacterHealthChange(CharacterHealthChangeEvent* event) override;
    // Djikstra Visualization functions.
    void DjikstaInitialSetup(DjikstraSearchEvent* event);
    void DjikstraVisualizeLoop(DjikstraSearchEvent* event, DjikstraSearchEvent::Loop loop, int loopId);
    void saveGame();
};

#endif // GRAPHICALUI_H
