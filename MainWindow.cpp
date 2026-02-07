#include "MainWindow.h"
#include <QLineEdit>
#include <QPushButton>
#include <QSlider>
#include <Qlabel>
#include "DungeonCrawler.h"
#include "GraphicalUI.h"
#include "ui_MainWindow.h"
#include "QGameField.h"
#include <QTextEdit>
#include <QGraphMatrix.h>
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qtoolbar.h>
#include <sstream>
#include "QGameOver.h"
#include "QTerminal.h"
#include "QGameWon.h"
MainWindow::MainWindow(Level *lvl, GraphicalUI *g, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , level{lvl}
    , GUI{g}
{
    // ui->setupUi(this);

    QWidget* central = new QWidget(this);
    setCentralWidget(central);

    QToolBar* toolbar = addToolBar("Save");
    QAction* saveGame = new QAction("Save Game");
    saveGame->setObjectName("saveGameButton");

    QFont font;
    font.setPixelSize(15);
    saveGame->setFont(font);
    toolbar->addAction(saveGame);
    QHBoxLayout* mainLayout = new QHBoxLayout(central);

    m_gameField = new QGameField();

    QWidget* rightSideContainer = new QWidget();
    // rightSideContainer->setFixedWidth(400);
    rightSideContainer->setFixedSize(400, 800);
    m_arrowField = new QWidget();

    mainLayout->addWidget(m_gameField);
    mainLayout->addWidget(rightSideContainer, 0, Qt::AlignTop);

    QVBoxLayout* rightSideLayout = new QVBoxLayout(rightSideContainer);

    QComboBox* visualizationOption =new QComboBox();
    visualizationOption->addItem("Full visualization");
    visualizationOption->addItem("Full visualization without text");
    visualizationOption->addItem("Only Final path");
    visualizationOption->addItem("None");
    connect(visualizationOption, &QComboBox::currentIndexChanged,
            this, [g,this](int index) {
                switch (index) {
                case 0:
                    g->setVisualizationMode(GraphicalUI::FullVisualization);
                    break;
                case 1:
                    g->setVisualizationMode(GraphicalUI::FullVisualizationWithoutText);
                    break;
                case 2:
                    g->setVisualizationMode(GraphicalUI::OnlyFinalPath);
                    break;
                case 3 :
                    g->setVisualizationMode(GraphicalUI::None);
                    break;

                }
            });

    QWidget* algorithmExplainerContainer = new QWidget();

    m_arrowField = new QWidget();
    QGridLayout* arrowsLayout = new QGridLayout(m_arrowField);
    generateArrowButtons(arrowsLayout);

    rightSideLayout->addWidget(visualizationOption);
    rightSideLayout->addWidget(algorithmExplainerContainer);
    generateVisualizationWidgets(algorithmExplainerContainer);
    rightSideLayout->addWidget(m_arrowField);

    m_gameBoard = new QGridLayout(m_gameField);
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


