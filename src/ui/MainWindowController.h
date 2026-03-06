#ifndef MAINWINDOWCONTROLLER_H
#define MAINWINDOWCONTROLLER_H

#include <QObject>
class MainWindow;
class QStackedWidget;

// This class controls which page is displayed on the main window, it connects to all pages, and listens on their state/
// specific buttons clicked and updates the main window

class MainWindowController : public QObject
{
    Q_OBJECT
private:
    MainWindow* m_mainWindow = nullptr;
    QStackedWidget* m_appScreens = nullptr;

    void createAppScreens();

public:
    MainWindowController();
};

#endif // MAINWINDOWCONTROLLER_H
