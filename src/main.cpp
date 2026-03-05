#include <QApplication>
#include "Constants.h"
#include "MainWindowController.h"
#include <qDebug>
int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    MainWindowController mainWindowController;
    return a.exec();
}
