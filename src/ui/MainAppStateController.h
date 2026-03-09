#ifndef MAINAPPSTATECONTROLLER_H
#define MAINAPPSTATECONTROLLER_H

#include <QObject>
#include <qmap.h>
class MainWindow;
class QStackedWidget;
class AppPageController;

// This class is responsible for the state of the app generally, it creates the main window
// and then sets its start screen through createPageControllers and createMainButtons.

// createPageControllers creates the controller for each page and saves it, then createStartScreen() creates the buttons
// responsible for choosing the first page.

// createStartScreen Buttons are of custom type PageNavigatorButton, which connect to the controllers actions
// fetched through (AppPageController:getActions()), the action tells the controller to setup its view, and then
// finally the button emits a showWidget() signal connected to the mainWindow setCentralWidget().

class MainAppStateController : public QObject
{
    Q_OBJECT
private:
    QList<AppPageController*> m_pageControllers{};

    MainWindow* m_mainWindow = nullptr;

    void createPageControllers();
    void createStartScreen();
public slots:

public:
    MainAppStateController();
};

#endif // MAINAPPSTATECONTROLLER_H
