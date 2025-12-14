#ifndef STATIONARYCONTROLLER_H
#define STATIONARYCONTROLLER_H
#include "AbstractUI.h"
#include "Level.h"
class Character;
class StationaryController : public AbstractController
{
    Character* m_character;
public:
    StationaryController();
    void attachCharacter(Character* character);

    int move(Level* currentLevel) override;


};

#endif // STATIONARYCONTROLLER_H
