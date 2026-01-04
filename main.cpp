#include <iostream>
#include "header.h"
#include "Tile.h"
#include "Level.h"
#include "AbstractUI.h"
#include "DungeonCrawler.h"
#include "TerminalUI.h"
#include "startscreen.h"
#include "GraphicalUI.h"
#include <QApplication>
#include "list.h"
#include "list.cpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DungeonCrawler dc;
    return a.exec();
}
