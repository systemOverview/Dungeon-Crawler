#include <QApplication>
#include "DungeonCrawler.h"
#include "GraphicalUI.h"
#include <qDebug>
#include <nlohmann/json.hpp>
using json = nlohmann::json;






int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DungeonCrawler dc;
    return a.exec();
}

