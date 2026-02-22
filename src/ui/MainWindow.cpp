#include "MainWindow.h"
#include <QGraphMatrix.h>
#include <QKeyEvent>
#include <QLineEdit>
#include <QPushButton>
#include <QSlider>
#include <QTextEdit>
#include <Qlabel>
#include <QtCore/qtimer.h>
#include <QtWidgets/qbuttongroup.h>
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qgroupbox.h>
#include <QtWidgets/qtoolbar.h>
#include <QtWidgets/qtoolbutton.h>
#include "ButtonItem.h"
#include "DungeonCrawler.h"
#include "GraphicalUI.h"
#include "QGameField.h"
#include "QGameOver.h"
#include "QGameWon.h"
#include "QTerminal.h"
#include "ui_MainWindow.h"
#include <TileItem.h>
#include <sstream>

void MainWindow::makeStartScreen() {
    QWidget* central = new QWidget();
    QHBoxLayout* mainLayout = new QHBoxLayout();
    central->setLayout(mainLayout);

    m_scene = new QGraphicsScene();
    m_view = new QGraphicsView();
    m_view->installEventFilter(this);
    m_view->setFrameStyle(QFrame::NoFrame);
    m_view->setScene(m_scene);
    mainLayout->addWidget(m_view);

    m_sidebar = new QWidget();
    m_sidebarLayout = new QVBoxLayout();
    m_sidebar->setLayout(m_sidebarLayout);

    m_sidebarToolBox = new QToolBox;
    m_sidebarLayout->addWidget(m_sidebarToolBox);

    mainLayout->addWidget(m_sidebar);

    m_character = new CharacterItem(CharacterItem::CharacterType::Human);

    createCharacterCustomizationOptions();
    m_sidebarToolBox->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    m_sidebar->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    mainLayout->setStretch(0, 2);
    mainLayout->setStretch(1, 1);

    ButtonItem* startButton = new ButtonItem(QPixmap(GUIPaths::StartButton),
                                             ButtonItem::ButtonType::StartButton);
    connect(startButton, &ButtonItem::buttonClicked, this, &MainWindow::startGame);
    startButton->setParentItem(m_character);
    startButton->setPos((m_character->boundingRect().width() / 2)
                            - startButton->boundingRect().width() / 2,
                        m_character->boundingRect().height() + 20);
    m_scene->addItem(m_character);

    setCentralWidget(central);
}

void MainWindow::createCharacterCustomizationOptions() {
    for (int i = 0; i < int(CharacterItem::CharacterPart::PAST_END); i++) {
        CharacterItem::CharacterPart characterPart = CharacterItem::CharacterPart(i);
        QWidget* options = new QWidget();
        QGridLayout* optionsLayout = new QGridLayout();

        options->setLayout(optionsLayout);

        QButtonGroup* optionsButtonGroup = new QButtonGroup();
        std::vector<QPixmap> variants = SpriteManager::GetIdleFrameVariants(
            CharacterItem::CharacterType::Human,
            characterPart,
            SpriteManager::ImageProcessingMode::TrimTransparent);
        int counter = 0;
        for (QPixmap& optionImage : variants) {
            QPushButton* optionButton = new QPushButton();
            optionButton->setStyleSheet(
                "border-color: gray; border-width : 2px; background-color:white");
            optionImage = optionImage.scaledToHeight(100);

            QIcon optionIcon(optionImage);
            optionButton->setIcon(optionIcon);
            optionButton->setIconSize({50, 100});

            optionsButtonGroup->addButton(optionButton, counter);
            optionsLayout->addWidget(optionButton, counter / 3,
                                     (counter) % 3); // 3 images per row

            connect(optionButton, &QPushButton::clicked, this, [counter, characterPart, this]() {
                characterCustomizationClicked(characterPart, counter);
            });
            QLabel l;
            counter++;
        }
        m_sidebarToolBox->addItem(options,
                                  CharacterWearables::CustomizationButtonsTexts.at(
                                      CharacterItem::CharacterPart(i)));
    }
}


qreal MainWindow::calculateTextureDimension() {
    return std::min(m_view->rect().width() / 10, m_view->rect().height() / 10);
}

void MainWindow::characterCustomizationClicked(CharacterItem::CharacterPart characterPart,
                                               int whichOption) {
    m_character->assignPart(characterPart, whichOption);
}

void MainWindow::startGame() {
    centralWidget()->layout()->removeWidget(m_sidebarToolBox);
    for (QGraphicsItem* item : m_scene->items()) {
        if (item != m_character) {
            m_scene->removeItem(item);
        }
    }
    m_sidebarToolBox->hide();
    emit gameStarted();
    return;
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent) {
    makeStartScreen();
    QTimer::singleShot(10, [this]() { startGame(); });
    // startGame(); //TEST TODO , refactor this to work with the state machines
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::move(std::pair<int, int> move) { GUI->move(move); }

QGridLayout* MainWindow::getGameBoard() const { return m_gameBoard; }
QGameField* MainWindow::getGameField() const { return m_gameField; }

QWidget* MainWindow::getArrowField() const { return m_arrowField; }

void MainWindow::prepareRightSideForVisualization() { m_arrowField->hide(); }

void MainWindow::generateVisualizationWidgets(QWidget* containingWidget) {
    QVBoxLayout* containingWidgetMainLayout = new QVBoxLayout(containingWidget);
    QWidget* buttonsContainer = new QWidget();
    QWidget* explainerContainer = new QWidget();
    QGraphMatrix* tileMatrixContainer = new QGraphMatrix();
    tileMatrixContainer->setObjectName("tileMatrixContainer");

    StaticQCharacter* explainerCharacter = new StaticQCharacter(
        ":/pics/textures/zombie/attacker.png");
    QTypeWriter* CharacterSpeech = new QTypeWriter();
    CharacterSpeech->setObjectName("algorithmStepExplainerField");
    CharacterSpeech->setReadOnly(true);

    QHBoxLayout* explainerContainerLayout = new QHBoxLayout(explainerContainer);
    explainerContainerLayout->addWidget(CharacterSpeech);
    explainerContainerLayout->addWidget(explainerCharacter);

    QPushButton* previousStepButton = new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::GoPrevious),
                                                      "Previous");
    QPushButton* pauseButton = new QPushButton(QIcon::fromTheme(
                                                   QIcon::ThemeIcon::MediaPlaybackPause),
                                               "Pause");
    QPushButton* nextStepButton = new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::GoNext),
                                                  "Next");

    QHBoxLayout* buttonsLayout = new QHBoxLayout(buttonsContainer);

    buttonsLayout->addWidget(previousStepButton);
    buttonsLayout->addWidget(pauseButton);
    buttonsLayout->addWidget(nextStepButton);

    containingWidgetMainLayout->addWidget(explainerContainer);
    containingWidgetMainLayout->addWidget(tileMatrixContainer);
    containingWidgetMainLayout->addWidget(buttonsContainer);
}

void MainWindow::gameOver() {
    auto currentRect = centralWidget()->rect();
    QGameOver* gameOverWidget = new QGameOver();
    gameOverWidget->setGeometry(currentRect);
    delete centralWidget();
    setCentralWidget(gameOverWidget);
    gameOverWidget->enable();
}

void MainWindow::gameWon() {
    auto currentRect = centralWidget()->rect();
    QGameWon* gameWonWidget = new QGameWon();
    gameWonWidget->setGeometry(currentRect);
    delete centralWidget();
    setCentralWidget(gameWonWidget);
    gameWonWidget->enable();
}

void MainWindow::showTerminal() {
    auto currentRect = centralWidget()->rect();
    QTerminal* terminal = new QTerminal();
    terminal->setGeometry(currentRect);
    delete centralWidget();
    setCentralWidget(terminal);
    terminal->enable();
}

TileItem* MainWindow::addTileToScene(int row, int col, char textureID) {
    TileItem* tile = new TileItem(row, col, textureID);
    m_scene->addItem(tile);
    connect(this, &MainWindow::dimensionsChanged, tile, &GameItem::resize);
    return tile;
}

void MainWindow::addCharacterToScene(CharacterItem* character) const {
    connect(this, &MainWindow::dimensionsChanged, character, &GameItem::resize);
    m_scene->addItem(character);
}

CharacterItem* MainWindow::getHumanCharachter() const { return m_character; }

bool MainWindow::eventFilter(QObject* obj, QEvent* event) {
    if (event->type() == QEvent::Resize) {
        QGraphicsView* view = qobject_cast<QGraphicsView*>(obj);
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (view) {
            GameItem::SetSideLength(calculateTextureDimension());
            emit dimensionsChanged();
        }
        m_scene->setSceneRect(m_scene->itemsBoundingRect());
    }
    return false;
}
