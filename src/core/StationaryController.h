#ifndef STATIONARYCONTROLLER_H
#define STATIONARYCONTROLLER_H
#include "AbstractController.h"
class Character;
class StationaryController : public AbstractController
{
public slots:
    void moveCharacter() override;

public:
    StationaryController(Character* controlledCharacter);
};

#endif // STATIONARYCONTROLLER_H
