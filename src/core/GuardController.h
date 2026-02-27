#ifndef GUARDCONTROLLER_H
#define GUARDCONTROLLER_H

#include "AbstractController.h"
class GuardController : public AbstractController
{
    Q_OBJECT
private:
public slots:
    void moveCharacter() override;

public:
    GuardController(Character* controlledCharacter);
};

#endif // GUARDCONTROLLER_H
