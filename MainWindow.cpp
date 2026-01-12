#include "MainWindow.h"
#include <QLineEdit>
#include <QPushButton>
#include <QSlider>
#include <Qlabel>
#include "DungeonCrawler.h"
#include "GraphicalUI.h"
#include "ui_MainWindow.h"
#include <sstream>

MainWindow::MainWindow(Level *lvl, GraphicalUI *g, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , level{lvl}
    , GUI{g}
{
    ui->setupUi(this);
    QWidget *gameField = ui->gameField;
    QWidget *arrowField = ui->arrowField;

    QGridLayout *gameLayout = new QGridLayout(gameField);
    QGridLayout *arrowsLayout = new QGridLayout(arrowField);
    gameBoard = gameLayout;
    // fill(gameLayout);
    arrowField->setMaximumHeight(300);
    generateArrowButtons(arrowsLayout);
}

void MainWindow::fill(QGridLayout *arrowsLayout)
{
    std::vector<std::vector<Tile *>> *tiles = level->getTiles();
    for (int i = 0; i < level->getHeight(); i++) {
        for (int j = 0; j < level->getWidth(); j++) {
            Tile *currentTile = (*tiles)[i][j];
            QWidget *tileWidget = new QWidget();
            tileWidget->setMinimumSize(50, 50);
            if (currentTile == nullptr) {
            }

            else {
                // cast into its type
                if (currentTile->hasCharacter()) {
                    std::ostringstream style;
                    style << "border-image : url(:" << "/pics/ textures/char/front/char_front_1.png"
                          << ") 0 0 0 0 stretch stretch;";

                    QString bgStyleFull = QString::fromStdString(style.str());
                    tileWidget->setStyleSheet(bgStyleFull);

                } else {
                    std::ostringstream style;
                    style << "border-image : url(" << currentTile->getTexturePath()
                          << ") 0 0 0 0 stretch stretch;";

                    QString bgStyleFull = QString::fromStdString(style.str());
                    tileWidget->setStyleSheet(bgStyleFull);
                }
            }

            arrowsLayout->addWidget(tileWidget, i, j);
        }
    }
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

MainWindow::~MainWindow()
{
    delete ui;
}

QGridLayout *MainWindow::getGameBoard()
{
    return gameBoard;
}
