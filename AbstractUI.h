//
// Created by MBK on 13.11.25.
//

#ifndef PRAK_ABSTRACTUI_H
#define PRAK_ABSTRACTUI_H
#include <iostream>
#include <QString>
class Level;
class Character;
class AbstractUI {
public:
    virtual void draw(Level*) = 0;
    virtual std::pair<int,int> translateMove(int step);
};

class AbstractController {
protected :
    Character* m_character;
public:
    // TODO : fix the move function across subclasses
    virtual std::pair<int,int> move() = 0 ;
    virtual void attachCharacter(Character* character);
    virtual std::pair<int,int> translateMove(int step);

};



#endif //PRAK_ABSTRACTUI_H
