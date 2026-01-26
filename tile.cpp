//
// Created by MBK on 12.11.25.
//

#include "Tile.h"
#include <QtCore/qdebug.h>

Tile::Tile(int row, int col, char texture, std::string texturePath) :
    m_row{row}, m_column{col}, m_texture{texture},  m_texturePath{texturePath}{}

Tile::~Tile() = default;

Tile* Tile::GenerateTile(char texture, int row, int column)
{
    switch(texture){
    case '.' : return new Floor(row, column);
    case '#' : return new Wall(row, column);
    case 'X' : return new Door(row,column);
    case '?' : return new Switch(row, column);
    case '_' : return new Pit(row, column);
    case '<' : return new Ramp(row, column);
    case '$' : return new LevelChanger(row, column);
    default : return new Floor(row, column);
        // players in the game string that generates the tiles are represented by different characters, Level handles generating them,
        // and we assume they are on a floor.
    }



    // else if (gameString[i] == 'X') {
    //     Door *door = new Door(row, column);
    //     doors.push_back(door);
    //     (tiles)[row][column] = door;
    //     m_graph->addVertex(door, 1);
    // }

    // else if (gameString[i] == '?') {
    //     Switch *switcher = new Switch(row, column);
    //     (tiles)[row][column] = switcher;
    //     theSwitch = switcher;
    //     m_graph->addVertex(switcher, 1);
    // }

    // else if (gameString[i] == '_') {
    //     Pit *pit = new Pit(row, column);
    //     (tiles)[row][column] = pit;
    //     m_graph->addVertex(pit, 1);
    // }

    // else if (gameString[i] == '<') {
    //     Ramp *ramp = new Ramp(row, column);
    //     (tiles)[row][column] = ramp;
    //     m_graph->addVertex(ramp, 1);
    // }

    // else if (gameString[i] == '$') {
    //     LevelChanger *levelChanger = new LevelChanger(row, column);
    //     (tiles)[row][column] = levelChanger;}

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

bool Tile::moveTo(Tile *desTile, Character *who)
{
    if (onLeave(desTile, who)) {
        std::pair<bool, Tile *> onEnterResult = desTile->onEnter(who);
        if (onEnterResult.first == false) {
            return false;
        }

        Tile *tileToMoveTo;

        if (onEnterResult.second != nullptr) {
            tileToMoveTo= onEnterResult.second; // if onEnter returns a non-nullptr as it's second argument, then it is the portal that the player will access
        } else {
            tileToMoveTo= desTile; // if onEnter returns a nullptr as it's second argument, then the desTile is a normal tile to be accessed
        }
        if (tileToMoveTo->hasCharacter()) {
            Character *characterAtWantedTile = tileToMoveTo->getCharacter();
            if (characterAtWantedTile->isHuman()!= who->isHuman()) { // then it means they are a human and a zombie, which means that they can fight
                who->attackPlayer(characterAtWantedTile);
                if (characterAtWantedTile->isAlive()) {
                    characterAtWantedTile->attackPlayer(who);

                } else {
                    who->setTile(tileToMoveTo);
                    tileToMoveTo->setCharacter(who);
                    setCharacter(nullptr);
                }
            }
        } else {
            who->setTile(tileToMoveTo);
            tileToMoveTo->setCharacter(who);
            setCharacter(nullptr);
        }
        return true;
    }
    return false;
}

bool Tile::onLeave(Tile *desTile, Character *who)
{
    return true;
}

std::pair<bool, Tile *> Tile::onEnter(Character *who)
{
    std::pair<bool, Tile *> result(isEntrable(), nullptr);
    return result;
}

std::string Tile::getTexturePath() const {return m_texturePath;}


Portal::Portal(int row, int column, int portalId)
    : Tile(row, column, 'O', "") {
    setPortalId(portalId);
}

bool Portal::isEntrable() { return true; }

void Portal::setPortal(Portal *portal) { portalToAccess = portal; }

void Portal::setPortalId(int portalId) {m_portalId = portalId;setTexturePath(m_portalTexturesRegister[portalId]);}

std::pair<bool, Tile *> Portal::onEnter(Character *who)
{
    std::pair<bool, Tile *> result(isEntrable(), portalToAccess);
    return result;
}

std::pair<bool, Tile *> Switch::onEnter(Character *who)
{
    activate();
    std::pair<bool, Tile *> result(isEntrable(), nullptr);
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

bool Pit::onLeave(Tile *desTile, Character *who)
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
