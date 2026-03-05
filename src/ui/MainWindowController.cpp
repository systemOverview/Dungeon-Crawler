#include "MainWindowController.h"
#include <QtWidgets/qstackedwidget.h>
#include "CustomLevelCreator.h"
#include "GameController.h"
#include <MainWindow.h>

void MainWindowController::createAppScreens() {
    m_appScreens = new QStackedWidget();
    GameController* gameController = new GameController();
    CustomLevelCreator* customLevelCreator = new CustomLevelCreator();

    int gameplayPageIndex = m_appScreens->addWidget(gameController->getGameBoardView());
    int levelCustomizerPageIndex = m_appScreens->addWidget(customLevelCreator);

    m_appScreens->setCurrentIndex(levelCustomizerPageIndex);

    m_mainWindow->setCentralWidget(m_appScreens);
}

MainWindowController::MainWindowController() {
    m_mainWindow = new MainWindow();
    createAppScreens();
    m_mainWindow->setCentralWidget(m_appScreens);
    m_mainWindow->show();
}
