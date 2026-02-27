//
// Created by MBK on 13.11.25.
//

#ifndef PRAK_CHARACTER_H
#define PRAK_CHARACTER_H
#include <QObject>
#include "AttackController.h"
#include "Constants.h"
#include "GuardController.h"
#include "HumanController.h"
#include "StationaryController.h"
#include <nlohmann/json.hpp>
class AbstractController;
class StationaryController;
using json = nlohmann::json;

class LevelGraph;
class QCharacter;
class StationaryController;
class Tile;
class Character : public QObject
{
    Q_OBJECT

protected:
    char m_texture;
    CharactersAttributes::Attributes m_attributes;
    int m_healthPoints = 0;

    Tile* m_currentTile = nullptr;
    AbstractController* m_controller = nullptr;

public slots:
    void setTile(Tile* newTile);
signals:
    void moved(Coordinates fromTileCoords, Coordinates ToTileCoords);

public:
    Character(char texture, CharactersAttributes::Attributes attributes, Tile* tile = nullptr)
        : m_texture(texture)
        , m_currentTile(tile)

    {
        m_attributes = attributes;
        m_healthPoints = attributes.initialHealthPoints;
    };

    static Character* GenerateCharacter(char texture,
                                        Tile* tile = nullptr,
                                        Level* level = nullptr,
                                        LevelGraph* levelGraph = nullptr);

    void setController(AbstractController *controller);

    char getTexture() const;
    bool isHuman() const;
    virtual AbstractController* getController() const;
    Tile* getTile() const;
    int getMaxHP() const;
    int getCurrentHP() const;
    int getStrength() const;
    bool isAlive() const;
    virtual ~Character();
};

// Start of subclasses
class Human : public Character
{
public:
    Human(char texture, Tile* tile = nullptr)
        : Character(texture, CharactersAttributes::HumanAttributes, tile) {
        m_controller = new HumanController(this);
    };
};

class Zombie : public Character
{

public:
    Zombie(char texture, Tile* tile = nullptr)
        : Character(texture, CharactersAttributes::ZombieAttributes, tile) {
        switch (texture) {
        case 'S':
            m_controller = new StationaryController(this);
            break;
        case 'G':
            m_controller = new GuardController(this);
            break;
        default : throw std::logic_error("Zombie type does not have an assigned controller. ");
        }
    };
};

class Attacker : public Character
{
public:
    Attacker(char texture, Level* level, LevelGraph* levelGraph, Tile* tile = nullptr)
        : Character(texture, CharactersAttributes::AttackerAttributes, tile) {
        m_controller = new AttackController(this, level, levelGraph);
    };
};

void to_json(json &jsonObject, const Character* characterObject);

#endif //PRAK_CHARACTER_H
