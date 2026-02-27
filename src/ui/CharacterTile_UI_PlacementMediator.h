#ifndef CHARACTERTILE_UI_PLACEMENTMEDIATOR_H
#define CHARACTERTILE_UI_PLACEMENTMEDIATOR_H

#include <QtCore/qpoint.h>
#include "Constants.h"
#include <map>
class TileItem;
class CharacterItem;
class CharacterTile_UI_PlacementMediator
// This (static) class takes care of characters placement on tiles.
// Discarded approaches that led to this approach:

// 1 - Setting positions from GraphicalUI or from the character by advancing on the Scene works fine on fixed screen sizes, but when
// the screen is resized, the character stays at the same position, but the tiles move, fixing that would involve ad-hoc math,
// more events/signals/slots.

// 2 - Setting the tile as a parent item of the character (making the char pos relative to the tile because then it becomes
// problematic to animate moving from one tile to another, the tile would have to detect when the character crosses its border
// (a charachter moves multiple times inside each tile) , change ownership when that happens, then the new tile would have to
// recalculate the character position and etc

{
private:
    struct CharacterPosition
    {
        Coordinates CoordinatesOfTile = {0, 0};
        Coordinates positionInsideTile = {0, 0};
        operator QString() const;
    };
    inline static std::map<CharacterItem*, CharacterPosition> CHARACTERS_POSITIONS;
    inline static constexpr int CELLS_PER_AXIS = 6;

    static CharacterPosition CalculateMove(const Coordinates& tileCoordinates,
                                           const Coordinates& innerCoordinates,
                                           const Coordinates& xyAdvance,
                                           int tilePastEndIndex,
                                           int innerPastEndIndex);

public:
    static void PlaceCharacterOnTile(CharacterItem* character, Coordinates coordinatesOfTile);
    static void AdvanceCharacter(CharacterItem* character, Coordinates xyAdvance);

    static QPointF GetCharacterPosition(CharacterItem* character);
};

#endif // CHARACTERTILE_UI_PLACEMENTMEDIATOR_H
