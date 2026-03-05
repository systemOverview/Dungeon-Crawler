#ifndef CHARACTERTILE_UI_PLACEMENTMEDIATOR_H
#define CHARACTERTILE_UI_PLACEMENTMEDIATOR_H

#include <QtCore/qpoint.h>
#include "Constants.h"
#include <map>
class CharacterItem;
class QGraphicsView;
class TileItem;
class GameBoardView;
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
    inline static constexpr int CELLS_PER_AXIS = 6;

    GameBoardView* m_gameBoardView = nullptr;

    std::map<CharacterItem*, CharacterPosition> m_charactersPositions = {};

    CharacterPosition calculateMove(const Coordinates& tileCoordinates,
                                    const Coordinates& innerCoordinates,
                                    const Coordinates& xyAdvance,
                                    int tilePastEndIndex,
                                    int innerPastEndIndex);

    QPointF calculateCharaterScenePosition(CharacterItem* character) const;

public:
    CharacterTile_UI_PlacementMediator(GameBoardView* gameBoardView);
    void placeCharacterOnTile(CharacterItem* character, Coordinates coordinatesOfTile);
    void advanceCharacter(CharacterItem* character, Coordinates xyAdvance);
    void recalculateCharactersScenePositions();
};

#endif // CHARACTERTILE_UI_PLACEMENTMEDIATOR_H
