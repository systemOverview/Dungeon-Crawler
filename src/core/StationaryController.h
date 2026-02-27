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

    std::pair<int, int> move() override;
};

#endif // STATIONARYCONTROLLER_H
