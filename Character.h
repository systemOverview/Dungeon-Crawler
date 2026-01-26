//
// Created by MBK on 13.11.25.
//

#ifndef PRAK_CHARACTER_H
#define PRAK_CHARACTER_H
#include "AbstractUI.h"
#include "EventBus.h"
#include "StationaryController.h"
#include "GuardController.h"
#include "AttackController.h"
class LevelGraph;
class QCharacter;
class StationaryController;
class Tile;
class TerminalUI;
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

    Character(char texture,int strength = 20 ,int stamina = 20, Tile* tile = nullptr )
        : m_texture(texture)
        , m_strength(strength)
        , m_stamina(stamina)
        , currentTile(tile)

    {

        m_hitPoints = getMaxHP();
    };

    static Character* GenerateCharacter(char texture,  Tile* tile=nullptr, Level* level = nullptr, LevelGraph* levelGraph = nullptr);

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
           Tile *tile = nullptr)
        : Character(texture, 10, 5, tile)
    {
        switch (texture){
        case 'S' : m_controller  = new class StationaryController(); m_texturePath = "/pics/textures/zombie/zombie_right.png"; break;
        case 'G' : m_controller = new class GuardController(); m_texturePath = "/pics/textures/zombie/assassin.png"; break;
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
           Tile *tile = nullptr)
        : Character(texture, 10, 5, tile)
    {
        m_texturePath = "/pics/textures/zombie/attacker.png";
        m_controller = new AttackController(level, levelGraph);
        m_controller->attachCharacter(this);
    };
};


#endif //PRAK_CHARACTER_H
