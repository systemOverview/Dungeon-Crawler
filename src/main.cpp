#include <QApplication>
#include <QObject>
#include "MainAppStateController.h"
#include <qDebug>

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    MainAppStateController mainController;
    return a.exec();
}
