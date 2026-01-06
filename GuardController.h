#ifndef GUARDCONTROLLER_H
#define GUARDCONTROLLER_H
#include "AbstractUI.h"
#include "tile.h"
#include "Level.h"

class GuardController : public AbstractController
{
private:
    std::vector<int> m_movementPath = {6,6,4,4,4,4,6,6};
    std::vector<int>::iterator m_movementPathIterator;
public:
    GuardController();
    std::pair<int,int> move() override;

};

#endif // GUARDCONTROLLER_H
