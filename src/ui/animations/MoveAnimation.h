#ifndef MOVEANIMATION_H
#define MOVEANIMATION_H
#include <QtCore/qtimeline.h>
#include "CharacterAnimation.h"
#include "Constants.h"
class CharacterTile_UI_PlacementMediator;
class MoveAnimation : public CharacterAnimation
{
    Q_OBJECT

private:
    Coordinates m_fromTileCoordinates;
    Coordinates m_toTileCoordinates;
    CharacterTile_UI_PlacementMediator* m_placementMediator = nullptr;

public slots:
    void playFrame(std::vector<int> frames, int iterator, Coordinates xyAdvancePerFrame);

public:
    void start() override;

    // void animateMove(Coordinates fromTileCoords, Coordinates toTileCoords);
    MoveAnimation(CharacterItem* character,
                  Coordinates fromTileCoords,
                  Coordinates toTileCoords,
                  CharacterTile_UI_PlacementMediator* placementMediator);
};

#endif // MOVEANIMATION_H
