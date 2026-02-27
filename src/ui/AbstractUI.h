//
// Created by MBK on 13.11.25.
//

#ifndef PRAK_ABSTRACTUI_H
#define PRAK_ABSTRACTUI_H
#include "AbstractController.h"

#include <QString>
#include <iostream>
class Level;
class Character;
class AbstractUI
{
public:
    virtual void draw(Level *) = 0;
    virtual std::pair<int, int> translateMove(int step);
};

#endif //PRAK_ABSTRACTUI_H
