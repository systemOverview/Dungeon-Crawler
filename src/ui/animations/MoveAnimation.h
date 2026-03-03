#ifndef MOVEANIMATION_H
#define MOVEANIMATION_H
#include <QtCore/qtimeline.h>
#include "CharacterAnimation.h"
#include "Constants.h"
class MoveAnimation : public CharacterAnimation
{
    Q_OBJECT

public:
protected:
private:
    Coordinates m_fromTileCoordinates;
    Coordinates m_toTileCoordinates;

public slots:
    void playFrame(std::vector<int> frames, int iterator, Coordinates xyAdvancePerFrame);

public:
    void start() override;

    // void animateMove(Coordinates fromTileCoords, Coordinates toTileCoords);
    MoveAnimation(CharacterItem* character, Coordinates fromTileCoords, Coordinates toTileCoords);
};

#endif // MOVEANIMATION_H
