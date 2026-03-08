//
// Created by MBK on 12.11.25.
//

#include "tile.h"
#include <QtCore/qdebug.h>
#include "PortalsConnector.h"

Tile::Tile(Types::TileType type, int row, int col)
    : m_tileType{type}
    , m_coordinates{row, col} {}


Types::TileType Tile::getTileType() const { return m_tileType; }

Character *Tile::getCharacter() const
{
    return character;
}

void Tile::setCharacter(Character *characterToPlace)
{
    character = characterToPlace;
}

int Tile::getRow() const { return m_coordinates.row; }

int Tile::getColumn() const { return m_coordinates.column; }

Coordinates Tile::getCoordinates() const { return m_coordinates; }

void Portal::setSiblingPortal(Portal* newSiblingPortal) {
    m_siblingPortal = newSiblingPortal;
}

;

Portal::Portal(int row, int column)
    : Tile(Types::TileType::Portal, row, column) {}

void Portal::setPortal(Portal* portal) { m_siblingPortal = portal; }

int Portal::getPortalId(){return m_portalId;}

Portal* Portal::getSiblingPortal()
{
    return m_siblingPortal;
}

void Portal::onPortalCreation(PortalCreationEvent *event)
{
    assert(m_siblingPortal==nullptr && "More than two portals with the same id"); // for now maximum amount of portals with the same id is 2
    setPortal(event->getCreatedPortal());
    event->getCreatedPortal()->setPortal(this);

}

/*door*/
void Door::switchDoorStatus() {
    if (m_tileType == Types::TileType::OpenDoor) {
        m_tileType = Types::TileType::ClosedDoor;
    }
    else {
        m_tileType = Types::TileType::OpenDoor;
    }
    emit tileTypeChanged(m_tileType);
}

// void to_json(json &jsonObject, const Tile* tileObject){
//     jsonObject = json {
//             {"texture",  tileObject->getTexture() },
//              {"row", tileObject->getRow()},
//              {"column", tileObject->getColumn()},
//     };
// }
Tile::~Tile() = default;
