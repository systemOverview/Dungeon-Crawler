//
// Created by MBK on 13.11.25.
//

#ifndef PRAK_CHARACTER_H
#define PRAK_CHARACTER_H
#include "AbstractUI.h"
#include "AttackController.h"
#include "GuardController.h"
#include "StationaryController.h"
#include "EventBus.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class LevelGraph;
class QCharacter;
class StationaryController;
class Tile;
class Character
{
protected:
    char m_texture;
    std::string m_texturePath = "/pics/textures/char/front/char_front_1.png";
    int m_strength{};
    int m_stamina{};
    int m_hitPoints{};
    Tile *currentTile; // The tile the player is at
    AbstractController* m_controller;
    QCharacter* m_QCharacter; // its QT widget.


public:

    Character(char texture,int strength = 20 ,int stamina = 20, Tile* tile = nullptr ,int hitPoints = -1)
        : m_texture(texture)
        , m_strength(strength)
        , m_stamina(stamina)
        , currentTile(tile)

    {
        if (hitPoints<0){
            // default is -1 (sentinel value) for starting a game with full power
            m_hitPoints = getMaxHP();
        }
        else{
            // otherwise a certain hp can be set to allow loading game state from file
            // without having to call decrementFromHP.
            m_hitPoints = hitPoints;
        }
    };

    static Character* GenerateCharacter(char texture, int hitPoints=-1,  Tile* tile=nullptr, Level* level = nullptr, LevelGraph* levelGraph = nullptr);

    AbstractUI *getTerminal();
    void setController(AbstractController *controller);

    char getTexture() const;
    std::string getTexturePath() const;
    bool isHuman() const;
    virtual AbstractController* getController() const;
    Tile* getTile() const;
    void setTile(Tile *newTile);
    virtual std::pair<int, int> move();
    int getMaxHP() const;
    int getCurrentHP() const;
    int getStrength() const;
    bool isAlive() const;
    void decrementFromHP(int amount);
    void attackPlayer(Character *characterToAttack);
    void setQCharacter(QCharacter* QCharacter);
    QCharacter* getQChatacter();
    ~Character();

};

class Zombie : public Character
{

public:

    Zombie(char texture,
           Tile *tile = nullptr, int hitPoints = -1)
        : Character(texture, 30, 5, tile, hitPoints)
    {
        switch (texture){
        case 'S' : m_controller  = new class StationaryController(); m_texturePath = "/pics/textures/zombie/zombie_right.png"; break;
        case 'G' : m_controller = new class GuardController(); m_texturePath = "/pics/greenzombie"; break;
        default : throw std::logic_error("Zombie type does not have an assigned controller. ");
        }
        if (m_controller){m_controller->attachCharacter(this);}
    };
};

class Attacker : public Character
{
public:
    Attacker(char texture,
             Level* level ,
             LevelGraph* levelGraph,
           Tile *tile = nullptr,
             int hitPoints = -1)
        : Character(texture, 10, 5, tile, hitPoints)
    {
        m_texturePath = "/pics/textures/zombie/attacker.png";
        m_controller = new AttackController(level, levelGraph);
        m_controller->attachCharacter(this);
    };
};

void to_json(json &jsonObject, const Character* characterObject);

#endif //PRAK_CHARACTER_H
