#ifndef STATIONARYCONTROLLER_H
#define STATIONARYCONTROLLER_H
#include "AbstractUI.h"
#include "Level.h"
class Character;
class StationaryController : public AbstractController
{
public:
    StationaryController();

    int move(Level* currentLevel) override;


};

#endif // STATIONARYCONTROLLER_H
