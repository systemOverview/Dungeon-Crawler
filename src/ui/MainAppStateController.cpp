#include "MainAppStateController.h"
#include <QtWidgets/qstackedwidget.h>
#include "CustomLevelCreator.h"
#include "GameController.h"
#include <MainWindow.h>
MainAppStateController::MainAppStateController() {
    m_mainWindow = new MainWindow();
    createAppScreens();
    m_mainWindow->setCentralWidget(m_appScreens);
    m_mainWindow->show();
}

void MainAppStateController::createAppScreens() {
    m_appScreens = new QStackedWidget();

    GameController* gameController = new GameController();
    CustomLevelCreator* customLevelCreator = new CustomLevelCreator();

    int gameplayPageIndex = m_appScreens->addWidget(gameController->getGameBoardView());
    int levelCustomizerPageIndex = m_appScreens->addWidget(customLevelCreator);

    m_appScreens->setCurrentIndex(levelCustomizerPageIndex);

    m_mainWindow->setCentralWidget(m_appScreens);

    // gameController->startNewGame(GameController::GameSource::DefaultLevels);

    connect(customLevelCreator,
            &CustomLevelCreator::finished,
            [this, gameController, gameplayPageIndex]() {
                gameController->startNewGame(GameController::GameSource::CustomLevels);
                m_appScreens->setCurrentIndex(gameplayPageIndex);
            });
}

void MainAppStateController::setDefaultSettings() {}
