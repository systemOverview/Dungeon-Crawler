#ifndef ATTACKCONTROLLER_H
#define ATTACKCONTROLLER_H
#include <QtCore/qdebug.h>
#include "AbstractController.h"
class Level;
class AttackController : public AbstractController
{
    Q_OBJECT
private:
    Coordinates getNextMove() override;

public:
    AttackController(Character* controlledCharacter);
};

#endif // ATTACKCONTROLLER_H
