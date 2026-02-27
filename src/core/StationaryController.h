#ifndef STATIONARYCONTROLLER_H
#define STATIONARYCONTROLLER_H
#include "AbstractController.h"
class Character;
class StationaryController : public AbstractController
{
private:
    Coordinates getNextMove() override;

public:
    StationaryController(Character* controlledCharacter);
};

#endif // STATIONARYCONTROLLER_H
