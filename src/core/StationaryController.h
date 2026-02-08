#ifndef STATIONARYCONTROLLER_H
#define STATIONARYCONTROLLER_H
#include "AbstractUI.h"
class Character;
class StationaryController : public AbstractController
{
public:
    StationaryController();

    std::pair<int, int> move() override;
};

#endif // STATIONARYCONTROLLER_H
