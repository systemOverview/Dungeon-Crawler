//
// Created by MBK on 13.11.25.
//

#ifndef PRAK_CHARACTER_H
#define PRAK_CHARACTER_H
#include <QObject>
#include "AbstractUI.h"
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
    std::string m_texturePath = "/pics/textures/char/front/char_front_1.png";
    Tile *currentTile; // The tile the player is at
    AbstractController* m_controller;
    QCharacter* m_QCharacter; // its QT widget.

    // new
    CharactersAttributes::Attributes m_attributes;
    int m_healthPoints = 0;

public slots:
    bool moveToTile(Tile* tile);
    virtual std::pair<int, int> move();
signals:
    void moved();

public:
    Character(char texture, CharactersAttributes::Attributes attributes, Tile* tile = nullptr)
        : m_texture(texture)
        , currentTile(tile)

    {
        m_attributes = attributes;
        m_healthPoints = attributes.initialHealthPoints;
    };

    static Character* GenerateCharacter(char texture,
                                        Tile* tile = nullptr,
                                        Level* level = nullptr,
                                        LevelGraph* levelGraph = nullptr);

    AbstractUI *getTerminal();
    void setController(AbstractController *controller);

    char getTexture() const;
    std::string getTexturePath() const;
    bool isHuman() const;
    virtual AbstractController* getController() const;
    Tile* getTile() const;
    void setTile(Tile *newTile);
    int getMaxHP() const;
    int getCurrentHP() const;
    int getStrength() const;
    bool isAlive() const;
    void decrementFromHP(int amount);
    void attackPlayer(Character *characterToAttack);
    void setQCharacter(QCharacter* QCharacter);
    QCharacter* getQChatacter();
    virtual ~Character();
};

// Start of subclasses
class Human : public Character
{
public:
    Human(char texture, Tile* tile = nullptr)
        : Character(texture, CharactersAttributes::HumanAttributes, tile) {
        m_controller = new HumanController();
    };
};

class Zombie : public Character
{

public:
    Zombie(char texture, Tile* tile = nullptr)
        : Character(texture, CharactersAttributes::ZombieAttributes, tile) {
        switch (texture) {
        case 'S':
            m_controller = new StationaryController();
            m_texturePath = "/pics/textures/zombie/zombie_right.png";
            break;
        case 'G':
            m_controller = new GuardController();
            m_texturePath = "/pics/greenzombie";
            break;
        default : throw std::logic_error("Zombie type does not have an assigned controller. ");
        }
        if (m_controller){m_controller->attachCharacter(this);}
    };
};

class Attacker : public Character
{
public:
    Attacker(char texture, Level* level, LevelGraph* levelGraph, Tile* tile = nullptr)
        : Character(texture, CharactersAttributes::AttackerAttributes, tile) {
        m_texturePath = "/pics/textures/zombie/attacker.png";
        m_controller = new AttackController(level, levelGraph);
        m_controller->attachCharacter(this);
    };
};

void to_json(json &jsonObject, const Character* characterObject);

#endif //PRAK_CHARACTER_H
