#include "MainWindow.h"
#include <QGridLayout>
#include <QtWidgets/qpushbutton.h>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent) {
    setMinimumSize(500, 500);
}

void MainWindow::setCentralWidget(QWidget* widget) { QMainWindow::setCentralWidget(widget); }

MainWindow::~MainWindow() {}
