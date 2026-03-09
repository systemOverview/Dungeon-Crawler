#include "MainAppStateController.h"
#include <QtWidgets/qstackedwidget.h>
#include "CustomLevelCreator.h"
#include "GameController.h"
#include "QGraphicsView"
#include <CharacterCustomizer.h>
#include <MainWindow.h>
#include <PageNavigatorButton.h>
#include <qgridlayout.h>
MainAppStateController::MainAppStateController() {
    m_mainWindow = new MainWindow();
    createPageControllers();
    createStartScreen();
    m_mainWindow->show();
}

void MainAppStateController::createPageControllers() {
    GameController* gameController = new GameController();
    CustomLevelCreator* customLevelCreator = new CustomLevelCreator();
    CharacterCustomizer* characterCustomizer = new CharacterCustomizer();

    m_pageControllers.push_back(gameController);
    m_pageControllers.push_back(customLevelCreator);
    m_pageControllers.push_back(characterCustomizer);

}

void MainAppStateController::createStartScreen() {
    QWidget* buttons = new QWidget(m_mainWindow);
    QGridLayout* buttonsLayout = new QGridLayout(buttons);

    for (AppPageController* pageController : m_pageControllers) {
        for (GameAction* action : pageController->getActions()) {
            PageNavigatorButton* button = new PageNavigatorButton(pageController->getWidget(),
                                                                  action);
            buttonsLayout->addWidget(button);
            connect(button,
                    &PageNavigatorButton::showWidget,
                    m_mainWindow,
                    &MainWindow::setCentralWidget);
        }
    }

    m_mainWindow->setCentralWidget(buttons);
}
