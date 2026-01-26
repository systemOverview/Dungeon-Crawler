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
#include <sstream>

MainWindow::MainWindow(Level *lvl, GraphicalUI *g, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , level{lvl}
    , GUI{g}
{
    // ui->setupUi(this);
    QWidget* central = new QWidget(this);
    setCentralWidget(central);

    QHBoxLayout* mainLayout = new QHBoxLayout(central);

    m_gameField = new QGameField();
    QWidget* rightSideContainer = new QWidget();
    // rightSideContainer->setFixedWidth(400);
    rightSideContainer->setFixedSize(400, 800);
    m_arrowField = new QWidget();

    mainLayout->addWidget(m_gameField);
    mainLayout->addWidget(rightSideContainer, 0, Qt::AlignTop);

    QVBoxLayout* rightSideLayout = new QVBoxLayout(rightSideContainer);

    QPushButton* stopVisualizationButton = new QPushButton();
    stopVisualizationButton->setText("Stop visualization");
    connect(stopVisualizationButton, &QPushButton::clicked, [g, this]() { g->quitVisualizationLoop();});

    QWidget* algorithmExplainerContainer = new QWidget();

    m_arrowField = new QWidget();
    QGridLayout* arrowsLayout = new QGridLayout(m_arrowField);
    generateArrowButtons(arrowsLayout);

    rightSideLayout->addWidget(stopVisualizationButton);
    rightSideLayout->addWidget(algorithmExplainerContainer);
    generateVisualizationWidgets(algorithmExplainerContainer);
    rightSideLayout->addWidget(m_arrowField);

    m_gameBoard = new QGridLayout(m_gameField);




    // m_gameField = new QGameField(); //

    // m_arrowField = new QWidget(); //
    // QHBoxLayout* hlayout = new QHBoxLayout(central); //
    // QVBoxLayout* vlayout = new QVBoxLayout(central); //

    // hlayout->addWidget(m_gameField); //
    // hlayout->addLayout(vlayout);

    // QPushButton* stopVisualizationButton = new QPushButton();
    // stopVisualizationButton->setMaximumWidth(300);
    // stopVisualizationButton->setText("Stop visualization");
    // connect(stopVisualizationButton, &QPushButton::clicked, [g, this]() { g->quitVisualizationLoop();});
    // vlayout->addWidget(stopVisualizationButton, Qt::AlignTop);

    // QLineEdit* algorithmStepExplainerField = new QLineEdit();
    // algorithmStepExplainerField->setReadOnly(true);
    // algorithmStepExplainerField->setObjectName("algorithmStepExplainerField");
    // algorithmStepExplainerField->setStyleSheet("QLineEdit {background-color: gray; color:white;}");
    // vlayout->addWidget(algorithmStepExplainerField); //

    // vlayout->addWidget(m_arrowField);


    // vlayout->addStretch();


    // m_arrowField->setFixedSize(300,300);


    // QGridLayout* gameLayout = new QGridLayout(m_gameField);
    // QGridLayout* arrowsLayout = new QGridLayout(m_arrowField);//
    // m_gameBoard = gameLayout;
    // m_arrowField->setMaximumHeight(300); //
    // generateArrowButtons(arrowsLayout); //
}/*
MainWindow::MainWindow(Level *lvl, GraphicalUI *g, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , level{lvl}
    , GUI{g}
{
    // ui->setupUi(this);
    QWidget* central = new QWidget(this);
    setCentralWidget(central);

    // QHBoxLayout* mainLayout = new QHBoxLayout(central);
    // mainLayout->addWidget(gameField);

    QGameField* gameField = new QGameField();
    m_gameField = gameField;

    m_arrowField = new QWidget();
    QHBoxLayout* hlayout = new QHBoxLayout(central);
    QVBoxLayout* vlayout = new QVBoxLayout(central);

    hlayout->addWidget(gameField);
    hlayout->addLayout(vlayout);

    QPushButton* stopVisualizationButton = new QPushButton();
    stopVisualizationButton->setMaximumWidth(300);
    stopVisualizationButton->setText("Stop visualization");
    connect(stopVisualizationButton, &QPushButton::clicked, [g, this]() { g->quitVisualizationLoop();});
    vlayout->addWidget(stopVisualizationButton, Qt::AlignTop);

    QLineEdit* algorithmStepExplainerField = new QLineEdit();
    algorithmStepExplainerField->setReadOnly(true);
    algorithmStepExplainerField->setObjectName("algorithmStepExplainerField");
    algorithmStepExplainerField->setStyleSheet("QLineEdit {background-color: gray; color:white;}");
    vlayout->addWidget(algorithmStepExplainerField);

    vlayout->addWidget(m_arrowField);


    vlayout->addStretch();


    m_arrowField->setFixedSize(300,300);


    QGridLayout* gameLayout = new QGridLayout(gameField);
    QGridLayout* arrowsLayout = new QGridLayout(m_arrowField);
    m_gameBoard = gameLayout;
    m_arrowField->setMaximumHeight(300);
    generateArrowButtons(arrowsLayout);
}*/


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


