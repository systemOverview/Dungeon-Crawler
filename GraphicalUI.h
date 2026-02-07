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
#include "Utilities.h"
#include "QGraphMatrix.h"
#include <QTextEdit>
#include "QTypeWriter.h"
class DungeonCrawler;
class GraphicalUI : public AbstractUI, public AbstractController, public EventListener
{
public:
    enum VisualizationMode { FullVisualization, FullVisualizationWithoutText, OnlyFinalPath, None };

private:
    bool m_isVisualizeModeOn=true;
    QDialog* startScreen;
    MainWindow *mainWindow;
    int currentWindow{0};
    Level* level;
    DungeonCrawler* dc;
    std::pair<int, int> lastMove;
    std::vector<QWidget *> m_healthBars;
    std::map<std::pair<int,int>, QTile*> m_Qtiles;
    QOverlay* m_overlayWidget = nullptr;
    QGraphMatrix* m_graphMatrix = nullptr;
    QTypeWriter* m_algorithmStepExplainerField = nullptr;
    VisualizationMode m_visualizationMode = FullVisualization;

public:
    void setVisualizationMode(VisualizationMode mode);
    GraphicalUI(Level *lvl, DungeonCrawler *d);
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
