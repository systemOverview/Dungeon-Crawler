#include "MainWindow.h"
#include <QGraphMatrix.h>
#include <QLineEdit>
#include <QPushButton>
#include <QSlider>
#include <QTextEdit>
#include <Qlabel>
#include <QtWidgets/qbuttongroup.h>
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qtoolbar.h>
#include "DungeonCrawler.h"
#include "GraphicalUI.h"
#include "GraphicsNavigator.h"
#include "QGameField.h"
#include "QGameOver.h"
#include "QGameWon.h"
#include "QTerminal.h"
#include "ui_MainWindow.h"
#include <sstream>

void MainWindow::makeStartScreen() {
    QWidget* central = new QWidget();
    QHBoxLayout* mainLayout = new QHBoxLayout();
    central->setLayout(mainLayout);

    m_startScene = new QGraphicsScene();
    m_startView = new QGraphicsView();
    m_startView->setFrameStyle(QFrame::NoFrame);

    showFullScreen();
    // this->setFixedSize(1500, 1000);
    m_startView->setScene(m_startScene);

    m_startView->setBackgroundBrush(Qt::black);
    QWidget* w = new QWidget();
    QGridLayout* layout = new QGridLayout();
    w->setStyleSheet("background-color:white");
    QPixmap pix(":/armors/textures/arrows/Image.png");
    pix = pix.scaled({66, 135});
    QLabel* cc = new QLabel();
    cc->setPixmap(pix);

    QPixmap arrow(":/arrows/textures/arrows/arrowrighttrimmed.png");
    // arrow = arrow.scaled(50, 50);
    QLabel* a = new QLabel();
    a->setPixmap(arrow);
    // a->setFixedSize(50, 50);

    w->setLayout(layout);
    layout->addWidget(cc, 0, 0, 2, 1);
    layout->addWidget(a, 0, 1);

    m_startScene->addWidget(w);

    w->setStyleSheet("background-color:black");

    mainLayout->addWidget(m_startView);

    // buttons testing

    m_sidebar = new QWidget();
    m_sidebarLayout = new QVBoxLayout();
    m_sidebar->setLayout(m_sidebarLayout);

    // mainLayout->addWidget(m_sidebar);
    m_sidebarToolBox = new QToolBox;
    mainLayout->addWidget(m_sidebarToolBox);
    m_sidebar->setStyleSheet("background-color:white");
    m_character = new CharacterItem();
    m_startScene->addItem(m_character);
    createArmorOptions();
    m_sidebarToolBox->setMinimumSize({400, 400});
    m_sidebarToolBox->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    setCentralWidget(central);
}

void MainWindow::createArmorOptions() {
    m_armorOptions = new QWidget();
    QGridLayout* armorOptionsLayout = new QGridLayout();
    m_armorOptions->setLayout(armorOptionsLayout);

    QButtonGroup* armorOptionsButtonGroup = new QButtonGroup();
    int counter = 0;
    GraphicsNavigator navig(CharacterWearables::Armor);
    while (true) {
        QPushButton* armorOptionButton = new QPushButton();
        armorOptionButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        QImage im = navig.getNextTexture();
        // m_startScene->addItem(new QGraphicsPixmapItem(QPixmap::fromImage(im)));

        if (im == navig.outOfRangeTexture()) {
            break;
        }
        QPixmap armorScaled(QPixmap::fromImage(im).scaled({50, 50}));
        QIcon icon(armorScaled);
        armorOptionButton->setIcon(icon);
        armorOptionButton->setIconSize({50, 50});

        QAction* action = new QAction();
        // action->setData(counter);
        // armorOptionButton->setDefaultAction(action);

        armorOptionsButtonGroup->addButton(armorOptionButton, counter);
        armorOptionsLayout->addWidget(armorOptionButton);
        armorOptionsLayout->addWidget(armorOptionButton, counter / 3, (counter) % 3);
        counter++;
    }
    connect(armorOptionsButtonGroup,
            &QButtonGroup::idClicked,
            this,
            &MainWindow::armorButtonClicked);

    m_sidebarToolBox->addItem(m_armorOptions, "Choose your armor!");
}

MainWindow::MainWindow(Level *lvl, GraphicalUI *g, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , level{lvl}
    , GUI{g}
{
    makeStartScreen();
    // ui->setupUi(this);

    // QWidget* central = new QWidget(this);
    // setCentralWidget(central);

    // QToolBar* toolbar = addToolBar("Save");
    // QAction* saveGame = new QAction("Save Game");
    // saveGame->setObjectName("saveGameButton");
    // connect(saveGame, &QAction::triggered, [this, g]() { g->saveGame(); });

    // QFont font;
    // font.setPixelSize(15);
    // saveGame->setFont(font);
    // toolbar->addAction(saveGame);
    // QHBoxLayout* mainLayout = new QHBoxLayout(central);

    // m_gameField = new QGameField();

    // QWidget* rightSideContainer = new QWidget();
    // // rightSideContainer->setFixedWidth(400);
    // rightSideContainer->setFixedSize(400, 800);
    // m_arrowField = new QWidget();

    // mainLayout->addWidget(m_gameField);
    // mainLayout->addWidget(rightSideContainer, 0, Qt::AlignTop);

    // QVBoxLayout* rightSideLayout = new QVBoxLayout(rightSideContainer);

    // QComboBox* visualizationOption =new QComboBox();
    // visualizationOption->addItem("Full visualization");
    // visualizationOption->addItem("Full visualization without text");
    // visualizationOption->addItem("Only Final path");
    // visualizationOption->addItem("None");
    // connect(visualizationOption, &QComboBox::currentIndexChanged,
    //         this, [g,this](int index) {
    //             switch (index) {
    //             case 0:
    //                 g->setVisualizationMode(GraphicalUI::FullVisualization);
    //                 break;
    //             case 1:
    //                 g->setVisualizationMode(GraphicalUI::FullVisualizationWithoutText);
    //                 break;
    //             case 2:
    //                 g->setVisualizationMode(GraphicalUI::OnlyFinalPath);
    //                 break;
    //             case 3 :
    //                 g->setVisualizationMode(GraphicalUI::None);
    //                 break;

    //             }
    //         });

    // QWidget* algorithmExplainerContainer = new QWidget();

    // m_arrowField = new QWidget();
    // QGridLayout* arrowsLayout = new QGridLayout(m_arrowField);
    // generateArrowButtons(arrowsLayout);

    // rightSideLayout->addWidget(visualizationOption);
    // rightSideLayout->addWidget(algorithmExplainerContainer);
    // generateVisualizationWidgets(algorithmExplainerContainer);
    // rightSideLayout->addWidget(m_arrowField);

    // m_gameBoard = new QGridLayout(m_gameField);
}

void MainWindow::generateArrowButtons(QGridLayout *arrowsField)
{
    int counter = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            QPushButton *arrow = new QPushButton;

            std::ostringstream style;
            std::pair<int, int> xymove = arrows[counter]->getMove();
            style << "border-image : url(" << arrows[counter]->getPath()
                  << ") 0 0 0 0 stretch stretch; height: 70px; width: 70px";
            QString bgStyleFull = QString::fromStdString(style.str());
            arrow->setStyleSheet(bgStyleFull);
            connect(arrow, &QPushButton::clicked, [xymove, this]() { move(xymove); });
            arrowsField->addWidget(arrow, i, j);
            counter++;
        }
    }
}
void MainWindow::move(std::pair<int, int> move)
{
    GUI->move(move);
}

QGameField *MainWindow::getGameField() const
{
    return m_gameField;
}

QWidget *MainWindow::getArrowField() const
{
    return m_arrowField;
}

MainWindow::~MainWindow()
{
    delete ui;
}

QGridLayout *MainWindow::getGameBoard() const
{
    return m_gameBoard;
}

void MainWindow::prepareRightSideForVisualization()
{
    m_arrowField->hide();
}
void MainWindow::generateVisualizationWidgets(QWidget *containingWidget)
{
    QVBoxLayout* containingWidgetMainLayout = new QVBoxLayout(containingWidget);
    QWidget* buttonsContainer = new QWidget();
    QWidget* explainerContainer = new QWidget();
    QGraphMatrix* tileMatrixContainer = new QGraphMatrix();
    tileMatrixContainer->setObjectName("tileMatrixContainer");


    StaticQCharacter* explainerCharacter = new StaticQCharacter(":/pics/textures/zombie/attacker.png");
    QTypeWriter* CharacterSpeech = new QTypeWriter();
    CharacterSpeech->setObjectName("algorithmStepExplainerField");
    CharacterSpeech->setReadOnly(true);

    QHBoxLayout* explainerContainerLayout = new QHBoxLayout(explainerContainer);
    explainerContainerLayout->addWidget(CharacterSpeech);
    explainerContainerLayout->addWidget(explainerCharacter);



    QPushButton* previousStepButton = new QPushButton (QIcon::fromTheme(QIcon::ThemeIcon::GoPrevious), "Previous");
    QPushButton* pauseButton = new QPushButton (QIcon::fromTheme(QIcon::ThemeIcon::MediaPlaybackPause), "Pause");
    QPushButton* nextStepButton = new QPushButton (QIcon::fromTheme(QIcon::ThemeIcon::GoNext), "Next");

    QHBoxLayout* buttonsLayout = new QHBoxLayout(buttonsContainer);

    buttonsLayout->addWidget(previousStepButton);
    buttonsLayout->addWidget(pauseButton);
    buttonsLayout->addWidget(nextStepButton);

    containingWidgetMainLayout->addWidget(explainerContainer);
    containingWidgetMainLayout->addWidget(tileMatrixContainer);
    containingWidgetMainLayout->addWidget(buttonsContainer);





}

void MainWindow::gameOver()
{
    auto currentRect = centralWidget()->rect();
    QGameOver* gameOverWidget = new QGameOver();
    gameOverWidget->setGeometry(currentRect);
    delete centralWidget();
    setCentralWidget(gameOverWidget);
    gameOverWidget->enable();
}

void MainWindow::gameWon()
{
    auto currentRect = centralWidget()->rect();
    QGameWon* gameWonWidget = new QGameWon();
    gameWonWidget->setGeometry(currentRect);
    delete centralWidget();
    setCentralWidget(gameWonWidget);
    gameWonWidget->enable();

}

void MainWindow::showTerminal()
{
    auto currentRect = centralWidget()->rect();
    QTerminal* terminal = new QTerminal();
    terminal->setGeometry(currentRect);
    delete centralWidget();
    setCentralWidget(terminal);
    terminal->enable();
}

void MainWindow::arrowClicked(MoveDirection moveDirection) {}

void MainWindow::armorButtonClicked(int armorID) {}
