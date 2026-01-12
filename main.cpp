#include <QApplication>
#include "DungeonCrawler.h"
#include "GraphicalUI.h"
#include "header.h"
#include "list.cpp"
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DungeonCrawler dc;
    return a.exec();
}
