#ifndef CHARACTERTILE_UI_PLACEMENTMEDIATOR_H
#define CHARACTERTILE_UI_PLACEMENTMEDIATOR_H

#include <QtCore/qpoint.h>
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
    inline static std::map<std::pair<int, int>, TileItem*> m_tiles = {};
    inline static std::map<std::pair<int, int>, CharacterItem*> m_characters = {};
    inline static std::map<CharacterItem*, std::pair<TileItem*, int>> CHARACTERS_POSITIONS;
    inline static constexpr int SUBCOLUMNS_PER_TILE = 6;

    static std::pair<TileItem*, int> CalculateNewCharacterPosition(
        std::pair<TileItem*, int> currentPosition);

public:
    static void AddTile(TileItem* tile);
    static void PlaceCharacterOnTile(CharacterItem* character, TileItem* tile);
    static void AdvanceCharacter(CharacterItem* character, int xAdvance = 0, int yAdvance = 0);

    static QPointF GetCharacterPosition(CharacterItem* character);
};

#endif // CHARACTERTILE_UI_PLACEMENTMEDIATOR_H
