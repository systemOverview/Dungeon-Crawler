#include <QApplication>
#include "DungeonCrawler.h"
#include "GraphicalUI.h"
#include "list.cpp"
#include <qDebug>
#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>
#include "JsonObjects.h"
using json = nlohmann::json;






int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // DungeonCrawler dc;
    MainWindow mw(nullptr, nullptr);
    mw.show();
    return a.exec();
}

