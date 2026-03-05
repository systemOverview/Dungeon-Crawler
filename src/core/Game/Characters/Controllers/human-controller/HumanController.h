#ifndef HUMANCONTROLLER_H
#define HUMANCONTROLLER_H

#include <QObject>
#include "AbstractController.h"

class HumanController : public AbstractController
{
    Q_OBJECT
private:
    Coordinates getNextMove() override;

public:
    HumanController(Character* controlledCharacter);
};

#endif // HUMANCONTROLLER_H
