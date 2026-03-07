#include <QApplication>
#include "MainAppStateController.h"
#include <qDebug>
int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    MainAppStateController mainWindowController;
    return a.exec();
}
