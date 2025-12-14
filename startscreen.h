#ifndef STARTSCREEN_H
#define STARTSCREEN_H
#include "GraphicalUI.h"
#include <QDialog>
class GraphicalUI;
namespace Ui {
class StartScreen;
}

class StartScreen : public QDialog
{
    Q_OBJECT

public:
    explicit StartScreen(GraphicalUI* GUI, QWidget *parent = nullptr);
    ~StartScreen();

private:
    Ui::StartScreen *ui;
    GraphicalUI* graphicalUI;
    void switchWindow();
};

#endif // STARTSCREEN_H
