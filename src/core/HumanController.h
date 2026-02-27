#ifndef HUMANCONTROLLER_H
#define HUMANCONTROLLER_H

#include <QObject>
#include "AbstractController.h"

class HumanController : public AbstractController
{
    Q_OBJECT
public slots:
    void moveCharacter() override;

public:
    HumanController(Character* controlledCharacter);
    std::pair<int, int> move() override;
};

#endif // HUMANCONTROLLER_H
