#include "CharacterTile_UI_PlacementMediator.h"
#include "TileItem.h"

#include "CharacterItem.h"
#include "GameBoardView.h"
#include "Utilities.h"

CharacterTile_UI_PlacementMediator::CharacterTile_UI_PlacementMediator(GameBoardView* gameBoardView)
    : m_gameBoardView{gameBoardView} {}
void CharacterTile_UI_PlacementMediator::placeCharacterOnTile(CharacterItem* character,
                                                              Coordinates coordinatesOfTile) {
    CharacterPosition characterPosition{coordinatesOfTile, Coordinates{0, 0}};
    m_charactersPositions.insert_or_assign(character, characterPosition);
    character->updatePosition(calculateCharaterScenePosition(character));
}
CharacterTile_UI_PlacementMediator::CharacterPosition
CharacterTile_UI_PlacementMediator::calculateMove(const Coordinates& tileCoordinates,
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
void CharacterTile_UI_PlacementMediator::advanceCharacter(CharacterItem* character,
                                                          Coordinates xyAdvance) {
    CharacterPosition currentPosition = m_charactersPositions.at(character);
    CharacterPosition newPosition = calculateMove(currentPosition.CoordinatesOfTile,
                                                  currentPosition.positionInsideTile,
                                                  xyAdvance,
                                                  GameSettings::TILES_PER_SIDE,
                                                  CELLS_PER_AXIS);

    m_charactersPositions.insert_or_assign(character, newPosition);

    character->updatePosition(calculateCharaterScenePosition(character),
                              CharacterItem::PositionUpdateReason::CharacterMovement);
}

void CharacterTile_UI_PlacementMediator::recalculateCharactersScenePositions() {
    for (auto [character, coordinates] : m_charactersPositions) {
        character->updatePosition(calculateCharaterScenePosition(character));
    }
}

QPointF CharacterTile_UI_PlacementMediator::calculateCharaterScenePosition(
    CharacterItem* character) const {
    CharacterPosition currentRelativePosition = m_charactersPositions.at(character);
    auto [CoordinatesOfTile, positionWithinTile] = currentRelativePosition;

    const TileItem* tile = m_gameBoardView->getTileViewByCoordinates(CoordinatesOfTile);

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
