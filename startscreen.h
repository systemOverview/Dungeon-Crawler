#ifndef STARTSCREEN_H
#define STARTSCREEN_H
#include <QDialog>
#include "GraphicalUI.h"
#include "DungeonCrawler.h"
class GraphicalUI;
namespace Ui {
class StartScreen;
}

class StartScreen : public QDialog
{
    Q_OBJECT

public:
    explicit StartScreen(DungeonCrawler* DC, QWidget *parent = nullptr);
    ~StartScreen();

private:
    DungeonCrawler* m_dungeonCrawler = nullptr;
    Ui::StartScreen *ui;
    GraphicalUI *graphicalUI;
    void switchWindow(DungeonCrawler::GameSourceOption option);
    void paintEvent(QPaintEvent* event) override;
};

#endif // STARTSCREEN_H
