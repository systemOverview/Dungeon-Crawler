#ifndef OBSERVER_H
#define OBSERVER_H
#include <iostream>
class Observer
{
public:
    virtual void reactToChange(std::string changedMemberName="") = 0;
};


#endif // OBSERVER_H
