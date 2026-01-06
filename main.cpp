#include <iostream>
#include "header.h"
#include "DungeonCrawler.h"
#include "GraphicalUI.h"
#include <QApplication>
#include "list.cpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DungeonCrawler dc;
    return a.exec();
}
