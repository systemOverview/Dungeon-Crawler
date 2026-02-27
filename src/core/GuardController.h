#ifndef GUARDCONTROLLER_H
#define GUARDCONTROLLER_H

#include "AbstractController.h"
class GuardController : public AbstractController
{
    Q_OBJECT
private:
    std::vector<int> m_movementPath = {6, 6, 4, 4, 4, 4, 6, 6};
    std::vector<int>::iterator m_movementPathIterator;
public slots:
    void moveCharacter() override;

public:
    GuardController(Character* controlledCharacter);
    std::pair<int, int> move() override;
};

#endif // GUARDCONTROLLER_H
