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
    int stength{};
    int stamina{};
    int hitPoints{};
    Tile* currentTile; // The tile the player is at
    AbstractController* m_controller = new TerminalUI;

public:
    Character(std::string_view txt, std::string_view texturePath="", Tile* tile=nullptr) :
        m_texture (txt),
        m_texturePath(texturePath),
        currentTile (tile)
    {
    };
    AbstractUI* getTerminal();
    void setController (AbstractController* controller);

    std::string_view getTexture() const;
    std::string_view getTexturePath() const;
    virtual AbstractController* getController();
    Tile* getTile() const;
    void setTile(Tile* newTile);

    int move(Level *currentLevel, std::pair <int, int> xymove);



};

class Zombie : public Character {
public:
    Zombie(std::string_view txt, std::string_view texturePath, AbstractController* controller, Tile* tile=nullptr) :
        Character(txt, texturePath, tile)
    {
        m_controller = controller;
        m_controller->attachCharacter(this);
    };
};

#endif //PRAK_CHARACTER_H
