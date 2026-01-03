//
// Created by MBK on 13.11.25.
//

#ifndef PRAK_CHARACTER_H
#define PRAK_CHARACTER_H
#include "TerminalUI.h"
#include "AbstractUI.h"
#include <iostream>
class StationaryController;
class Tile;
class TerminalUI;
class Character{
protected:
    std::string m_texture;
    std::string m_texturePath;
    int m_strength{};
    int m_stamina{};
    int m_hitPoints{};
    bool m_isHuman {true};
    Tile* currentTile; // The tile the player is at
    AbstractController* m_controller = new TerminalUI;


public:
    Character(std::string_view txt, std::string_view texturePath, int strength, int stamina, Tile* tile=nullptr, bool isHuman=true) :
        m_texture (txt),
        m_texturePath(texturePath),
        currentTile (tile),
        m_strength(strength),
        m_stamina(stamina),
        m_isHuman(isHuman)
    {
        m_hitPoints = getMaxHP();
    };
    AbstractUI* getTerminal();
    void setController (AbstractController* controller);

    std::string_view getTexture() const;
    std::string_view getTexturePath() const;
    bool isHuman();
    virtual AbstractController* getController();
    Tile* getTile() const;
    void setTile(Tile* newTile);
    int move(Level *currentLevel, std::pair <int, int> xymove);
    int getMaxHP();
    int getCurrentHP();
    bool isAlive();
    void decrementFromHP(int amount);
    void attackPlayer(Character* characterToAttack);



};

class Zombie : public Character {
public:
    Zombie(std::string_view txt, std::string_view texturePath, int strength, int stamina, AbstractController* controller, Tile* tile=nullptr) :
        Character(txt,texturePath, strength, stamina, tile,false)
    {
        m_controller = controller;
        m_controller->attachCharacter(this);
    };
};

#endif //PRAK_CHARACTER_H
