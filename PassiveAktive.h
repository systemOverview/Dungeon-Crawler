//
// Created by MBK on 26.11.25.
//

#ifndef PRAK_PASSIVEAKTIVE_H
#define PRAK_PASSIVEAKTIVE_H
#include <iostream>
#include <vector>
class Passive
{
public:
    virtual void notify() = 0;
};

class Active
{
protected:
    std::vector<Passive *> passiveObjects{};

public:
    virtual void attach(Passive *);
    virtual void detach(Passive *);
    virtual void activate();
};

#endif //PRAK_PASSIVEAKTIVE_H
