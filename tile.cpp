//
// Created by MBK on 12.11.25.
//

#include "tile.h"
#include <QtCore/qdebug.h>

Tile::Tile(int row, int col, char texture, std::string texturePath, int djikstraExtraCost) :
    m_row{row}, m_column{col}, m_texture{texture},  m_texturePath{texturePath}, m_djikstraExtraCost{djikstraExtraCost}{}

Tile::~Tile() = default;

Tile* Tile::GenerateTile(char texture, int row, int column, int djikstraExtraCost)
{
    switch(texture){
    case '.' : return new Floor(row, column, djikstraExtraCost);
    case '#' : return new Wall(row, column, djikstraExtraCost);
    case 'X' : return new Door(row,column, true, djikstraExtraCost);
    case '/' : return new Door(row,column,false, djikstraExtraCost);
    case '?' : return new Switch(row, column, djikstraExtraCost);
    case '_' : return new Pit(row, column, djikstraExtraCost);
    case '<' : return new Ramp(row, column, djikstraExtraCost);
    case '$' : return new LevelChanger(row, column);
    case '!' : return new GameWinner (row, column);

    default :
    {
        if (texture-'0'<10){ // portals are identified by numbers
            std::cout << "texture" << texture-'0';
            return new Portal(row, column, texture-'0');
        }
        return new Floor(row, column, djikstraExtraCost);
    }

    }
}

char Tile::getTexture() const
{
    return m_texture;
}

bool Tile::hasCharacter() const
{
    return (character != nullptr);
}

Character *Tile::getCharacter() const
{
    return character;
}

void Tile::setCharacter(Character *characterToPlace)
{
    character = characterToPlace;
    EventBus::transmitEvent<EventBus::TileChange>(this, TileChangeEvent::Character);
}

int Tile::getRow() const
{
    return m_row;
}

int Tile::getColumn() const
{
    return m_column;
}

std::pair<int, int> Tile::getCordsAsPair() const
{
    return {m_row, m_column};
}


bool Tile::onLeave(Tile *desTile)
{
    return true;
}

std::pair<bool, Tile *> Tile::onEnter()
{
    std::pair<bool, Tile *> result(isEntrable(), this);
    return result;
}

std::string Tile::getTexturePath() const {return m_texturePath;}


Portal::Portal(int row, int column, int portalId)
    : Tile(row, column, 'O', "") {
    setPortalId(portalId);
    EventBus::transmitEvent<EventBus::PortalCreation>(this);
    EventBus::subscribeToEvent<EventBus::PortalCreation>(this, portalId);

}

bool Portal::isEntrable() { return true; }

void Portal::setPortal(Portal *portal) { m_siblingPortal = portal; }

void Portal::setPortalId(int portalId)
{
    m_portalId = portalId;
    setTexturePath(m_portalTexturesRegister[portalId%3]);}

int Portal::getPortalId(){return m_portalId;}

Portal* Portal::getSiblingPortal()
{
    return m_siblingPortal;
}

std::pair<bool, Tile *> Portal::onEnter()
{
    std::pair<bool, Tile *> result(isEntrable(), m_siblingPortal);
    return result;
}

void Portal::onPortalCreation(PortalCreationEvent *event)
{
    qDebug() << m_portalId;
    assert(m_siblingPortal==nullptr && "More than two portals with the same id"); // for now maximum amount of portals with the same id is 2
    setPortal(event->getCreatedPortal());
    event->getCreatedPortal()->setPortal(this);

}

std::pair<bool, Tile *> Switch::onEnter()
{
    activate();
    std::pair<bool, Tile *> result(isEntrable(), this);
    return result;
}

/*door*/
void Door::notify()
{
    state = !state;
    if (state) {
        m_texture = '/';
        setTexturePath(":/pics/textures/doors/door2.png");

    } else {
        m_texture = 'X';
        setTexturePath(":/pics/textures/doors/door1.png");
    }
    EventBus::transmitEvent<EventBus::TileChange>(this, TileChangeEvent::DoorStatus);

}

/*pit*/

bool Pit::onLeave(Tile *desTile)
{
    if (desTile->getTexture() != '<' && desTile->getTexture() != '_') {
        return false;
    }
    return true;
}



void Tile::setTexturePath(std::string texturePath){
    m_texturePath = texturePath;
    EventBus::transmitEvent<EventBus::TileChange>(this, TileChangeEvent::TextureChange);
}

int Tile::getDjikstraExtraCost() const
{
    return m_djikstraExtraCost;
}

void Tile::SetAllowedDjikstraValueChanges(int amount)
{
    AllowedDjikstraValueChanges = amount;
}

bool Tile::setDjikstraExtraCost(int cost)
{
    if (Tile::AllowedDjikstraValueChanges>0){
        m_djikstraExtraCost = cost;
        Tile::AllowedDjikstraValueChanges-=1;
        return true;
    }
    return false;
}

// Tile *Tile::generateTileFromJSONState(TileState &tile)
// {
//     return Tile::GenerateTile(tile.texture, tile.row, tile.col);
// }

void to_json(json &jsonObject, const Tile* tileObject){
    jsonObject = json {
            {"texture",  tileObject->getTexture() },
             {"row", tileObject->getRow()},
             {"column", tileObject->getColumn()},
               {"Djikstra Extra Cost", tileObject->getDjikstraExtraCost()}
    };
}
