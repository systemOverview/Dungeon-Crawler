//
// Created by MBK on 13.11.25.
//

#ifndef PRAK_ABSTRACTUI_H
#define PRAK_ABSTRACTUI_H
#include <iostream>
class Level;
class AbstractUI {
public:
    virtual void draw(Level*) = 0;
    virtual std::pair<int,int> translateMove(int step) = 0;

};

class AbstractController {
public:
    virtual int move() = 0 ;
};



#endif //PRAK_ABSTRACTUI_H