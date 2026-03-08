#ifndef MAINAPPSTATECONTROLLER_H
#define MAINAPPSTATECONTROLLER_H

#include <QObject>
class MainWindow;
class QStackedWidget;

// This class is responsible for the state of the app generally, it creates the main window,  and creates all the other windows
// controllers (which create their views/models). It listens on the signals emitted by the submain windows it created to switch
// between screens.

class MainAppStateController : public QObject
{
    Q_OBJECT
private:
    MainWindow* m_mainWindow = nullptr;
    QStackedWidget* m_appScreens = nullptr;

    void createAppScreens();
    void setDefaultSettings();

public:
    MainAppStateController();
};

#endif // MAINAPPSTATECONTROLLER_H
