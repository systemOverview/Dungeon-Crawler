//
// Created by MBK on 12.11.25.
//

#include "tile.h"
#include <QtCore/qdebug.h>
#include "PortalsConnector.h"

Tile::Tile(int row, int col, char texture)
    : m_coordinates{row, col}
    , m_texture{texture} {}


Tile* Tile::GenerateTile(char texture, int row, int column) {
    switch (texture) {
    case '.':
        return new Floor(row, column);
    case '#':
        return new Wall(row, column);
    case 'X': {
        return new Door(row, column, true);
    }
    case '/':
        return new Door(row, column, false);
    case '?':
        return new Switch(row, column);
    case '_':
        return new Pit(row, column);
    case '<':
        return new Ramp(row, column);
    case '$' : return new LevelChanger(row, column);
    case '!' : return new GameWinner (row, column);

    default :
    {
        if (texture-'0'<10){ // portals are identified by numbers
            return new Portal(row, column, texture-'0');
        }
        return new Floor(row, column);
    }
    }
}

char Tile::getTexture() const
{
    return m_texture;
}

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

void Portal::setSiblingPortal(Portal* newSiblingPortal) { m_siblingPortal = newSiblingPortal; }

Portal::Portal(int row, int column, int portalId)
    : Tile(row, column, std::to_string(portalId).at(0))
    , m_portalId{portalId} {
    PortalsConnector::AddPortal(this);
};

void Portal::setPortal(Portal* portal) { m_siblingPortal = portal; }

int Portal::getPortalId(){return m_portalId;}

Portal* Portal::getSiblingPortal()
{
    return m_siblingPortal;
}

void Portal::onPortalCreation(PortalCreationEvent *event)
{
    qDebug() << m_portalId;
    assert(m_siblingPortal==nullptr && "More than two portals with the same id"); // for now maximum amount of portals with the same id is 2
    setPortal(event->getCreatedPortal());
    event->getCreatedPortal()->setPortal(this);

}

/*door*/
void Door::alertOfAccess() {
    state = !state;
    if (state) {
        m_texture = '/';
    }
    else {
        m_texture = 'X';
    }
    emit textureChanged(m_texture);
}

void to_json(json &jsonObject, const Tile* tileObject){
    jsonObject = json {
            {"texture",  tileObject->getTexture() },
             {"row", tileObject->getRow()},
             {"column", tileObject->getColumn()},
    };
}
Tile::~Tile() = default;
