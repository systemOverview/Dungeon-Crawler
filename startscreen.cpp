#include "startscreen.h"
#include <QDialog>
#include <QPixmap>
#include <QtWidgets>
#include "ui_startscreen.h"
StartScreen::StartScreen(DungeonCrawler* DC, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::StartScreen), m_dungeonCrawler{DC}
{
    // graphicalUI = GUI;
    // ui->setupUi(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QHBoxLayout* layout = new QHBoxLayout();
    mainLayout->addStretch();
    mainLayout->addLayout(layout);
    QPushButton* startGame = new QPushButton("Start new game");
    QPushButton* loadFile = new QPushButton("Load previous game");
    this->setStyleSheet("QPushButton{background-color:red; color:white}");
    layout->addWidget(startGame);
    layout->addWidget(loadFile);

    connect(startGame, &QPushButton::clicked, [this]() {switchWindow(DungeonCrawler::FromStrings);});
    connect(loadFile, &QPushButton::clicked, [this]() {switchWindow(DungeonCrawler::FromFile);});

    setFixedSize(1000,600);

}

StartScreen::~StartScreen()
{
    delete ui;
}

void StartScreen::switchWindow(DungeonCrawler::GameSourceOption option)
{
    m_dungeonCrawler->buildGame(option);
}

void StartScreen::paintEvent(QPaintEvent *event)
{
    QPainter painter (this);
    QPixmap background (":/pics/textures/start screen/startscreen_dark.png");
    painter.drawPixmap(rect(), background);
    QDialog::paintEvent(event);
}
