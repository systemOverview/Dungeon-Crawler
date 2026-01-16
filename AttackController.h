#ifndef ATTACKCONTROLLER_H
#define ATTACKCONTROLLER_H
#include "AbstractUI.h"
class AttackController : public AbstractController
{
public:
    AttackController();
    std::pair<int, int> move() override;

};

#endif // ATTACKCONTROLLER_H
