#include "CharacterTile_UI_PlacementMediator.h"
#include "TileItem.h"

void CharacterTile_UI_PlacementMediator::AddTile(TileItem* tile) {
    m_tiles.insert_or_assign(tile->getCordsAsPair(), tile);
}

void CharacterTile_UI_PlacementMediator::PlaceCharacterOnTile(CharacterItem* character,
                                                              TileItem* tile) {
    CHARACTERS_POSITIONS.insert_or_assign(character, std::pair{tile, 0});
    character->fixMyPosition();
}

void CharacterTile_UI_PlacementMediator::AdvanceCharacter(CharacterItem* character,
                                                          int xAdvance,
                                                          int yAdvance) {
    assert(CHARACTERS_POSITIONS.count(character) > 0
           && "Advancing a character with no assigned position");

    std::pair<TileItem*, int> tileSubColumnPair = CHARACTERS_POSITIONS.at(character);
    auto [tile, subcolumn] = tileSubColumnPair;

    CHARACTERS_POSITIONS[character] = CalculateNewCharacterPosition(tileSubColumnPair);

    character->fixMyPosition();

    // TODO handle yAdvance.
}

std::pair<TileItem*, int> CharacterTile_UI_PlacementMediator::CalculateNewCharacterPosition(
    std::pair<TileItem*, int> currentPosition) {
    std::pair<TileItem*, int> newPosition = currentPosition;

    auto [tile, subcolumn] = currentPosition;

    assert(subcolumn <= SUBCOLUMNS_PER_TILE && "Subcolumn overflow");

    if (subcolumn == SUBCOLUMNS_PER_TILE) {
        newPosition.first = m_tiles[{tile->getRow() + 1, tile->getColumn()}];
        newPosition.second = 0;
    }
    else {
        newPosition.first = currentPosition.first;
        newPosition.second = currentPosition.second + 1;
    }
    // TODO handle yAdvance and tile overflow.

    return newPosition;
}

QPointF CharacterTile_UI_PlacementMediator::GetCharacterPosition(CharacterItem* character) {
    std::pair<TileItem*, int> tileSubColumnPair = CHARACTERS_POSITIONS.at(character);

    auto [tile, subcolumn] = tileSubColumnPair;

    float subcolumnWidth = (tile->boundingRect().width() / SUBCOLUMNS_PER_TILE);
    QPointF tileScenePos = tile->mapToScene({0, 0});

    QPointF characterPos{tileScenePos.x() + (subcolumn * subcolumnWidth), tileScenePos.y()};
    return characterPos;
}
