#include "CharacterTile_UI_PlacementMediator.h"
#include "TileItem.h"

#include "GraphicalUI.h"
#include "Utilities.h"

void CharacterTile_UI_PlacementMediator::PlaceCharacterOnTile(CharacterItem* character,
                                                              Coordinates coordinatesOfTile) {
    CHARACTERS_POSITIONS.insert_or_assign(character,
                                          CharacterPosition{coordinatesOfTile, Coordinates{0, 0}});
}
CharacterTile_UI_PlacementMediator::CharacterPosition
CharacterTile_UI_PlacementMediator::CalculateMove(const Coordinates& tileCoordinates,
                                                  const Coordinates& innerCoordinates,
                                                  const Coordinates& xyAdvance,
                                                  int tilePastEndIndex,
                                                  int innerPastEndIndex) {
    Coordinates newTileCoordinates = tileCoordinates;
    Coordinates newInnerCoordinates = innerCoordinates + xyAdvance;

    int rowWraps = newInnerCoordinates.row / innerPastEndIndex;
    int colWraps = newInnerCoordinates.column / innerPastEndIndex;

    newTileCoordinates.row = Utilities::positiveModulo(newTileCoordinates.row + rowWraps,
                                                       tilePastEndIndex);
    newTileCoordinates.column = Utilities::positiveModulo(newTileCoordinates.column + colWraps,
                                                          tilePastEndIndex);

    newInnerCoordinates.row = newInnerCoordinates.row - rowWraps * innerPastEndIndex;
    newInnerCoordinates.column = newInnerCoordinates.column - colWraps * innerPastEndIndex;

    if (newInnerCoordinates.row < 0) {
        newInnerCoordinates.row = Utilities::positiveModulo(newInnerCoordinates.row,
                                                            innerPastEndIndex);
        newTileCoordinates.row = Utilities::positiveModulo(newTileCoordinates.row - 1,
                                                           tilePastEndIndex);
    }

    if (newInnerCoordinates.column < 0) {
        newInnerCoordinates.column = Utilities::positiveModulo(newInnerCoordinates.column,
                                                               innerPastEndIndex);
        newTileCoordinates.column = Utilities::positiveModulo(newTileCoordinates.column - 1,
                                                              tilePastEndIndex);
    }
    return {newTileCoordinates, newInnerCoordinates};
}
void CharacterTile_UI_PlacementMediator::AdvanceCharacter(CharacterItem* character,
                                                          Coordinates xyAdvance) {
    CharacterPosition currentPosition = CHARACTERS_POSITIONS.at(character);
    CharacterPosition newPosition = CalculateMove(currentPosition.CoordinatesOfTile,
                                                  currentPosition.positionInsideTile,
                                                  xyAdvance,
                                                  GameSettings::TILES_PER_SIDE,
                                                  CELLS_PER_AXIS);

    CHARACTERS_POSITIONS.insert_or_assign(character, newPosition);

    character->fixMyPosition();
}

QPointF CharacterTile_UI_PlacementMediator::GetCharacterPosition(CharacterItem* character) {
    CharacterPosition currentRelativePosition = CHARACTERS_POSITIONS.at(character);
    auto [CoordinatesOfTile, positionWithinTile] = currentRelativePosition;

    TileItem* tile = GraphicalUI::GetGraphicalTile(CoordinatesOfTile);

    float CellWidth = (tile->boundingRect().width() / CELLS_PER_AXIS);

    QPointF tileScenePos = tile->mapToScene({0, 0});

    QPointF characterPos{tileScenePos.x() + (positionWithinTile.column * CellWidth),
                         tileScenePos.y() + (positionWithinTile.row * CellWidth)};
    return characterPos;
}

CharacterTile_UI_PlacementMediator::CharacterPosition::operator QString() const {
    QString info;
    info += " is at " + CoordinatesOfTile + " with inner coordinates of " + positionInsideTile;
    return info;
}
