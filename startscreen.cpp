#include "startscreen.h"
#include "ui_startscreen.h"
#include <iostream>
#include <QPixmap>
#include <QtWidgets>
#include <QDialog>



StartScreen::StartScreen(GraphicalUI* GUI, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::StartScreen)
{
    graphicalUI = GUI;
    ui->setupUi(this);

    // this->setStyleSheet("background-image: :/pics/startscreen_dark.png;");
    // QPixmap bkgnd(":/pics/startscreen_dark.png");
    // bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    // QPalette palette;
    // palette.setBrush(QPalette::Window, bkgnd);
    // this->setPalette(palette);
    connect(ui->newGameButton, &QPushButton::clicked, this , &StartScreen::switchWindow);
    //     connect(ui->actionSearch, &QAction::triggered, this, &MainWindow::search);


}
// QWidget::setStyleSheet("background-image: :/textures/switch.png;");
//    redPixmap = new QPixmap(":/images/red.png");

StartScreen::~StartScreen()
{
    delete ui;
}

void StartScreen::switchWindow()
{
    graphicalUI->switchWindow();
}
